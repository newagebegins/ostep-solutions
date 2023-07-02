#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mapreduce.h"

#define Pthread_create(threadp, attrp, start_routinep, argp) assert(pthread_create(threadp, attrp, start_routinep, argp) == 0)
#define Pthread_join(thread, value_ptr) assert(pthread_join(thread, value_ptr) == 0)

#define Pthread_mutex_init(mutexp, attrp) assert(pthread_mutex_init(mutexp, attrp) == 0)
#define Pthread_mutex_lock(mutexp) assert(pthread_mutex_lock(mutexp) == 0)
#define Pthread_mutex_unlock(mutexp) assert(pthread_mutex_unlock(mutexp) == 0)

struct value {
  char* text;
  struct value* next;
};

struct key {
  char* id;
  unsigned long hash;
  struct value* values;
  struct value* used_values;
};

struct bucket {
  pthread_mutex_t lock;
  size_t capacity;
  size_t length;
  struct key* keys;
};

static int argc = 0;
static char** argv = 0;
static Mapper map = 0;
static Reducer reduce = 0;
static Partitioner partition = 0;
static int num_reducers = 0;

static pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
static size_t next_file = 1;

static struct bucket* buckets = 0;

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
  unsigned long hash = 5381;
  int c;
  while ((c = *key++) != '\0')
    hash = hash * 33 + c;
  return hash % num_partitions;
}

unsigned long hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

void MR_Emit(char *key, char *value) {
  unsigned long key_hash = hash((unsigned char*)key);
  unsigned long partition_number = partition(key, num_reducers);
  struct bucket* bucket = &buckets[partition_number];
  Pthread_mutex_lock(&bucket->lock);
  for (size_t i = 0; i < bucket->length; ++i) {
    struct key* p = &bucket->keys[i];
    if (key_hash == p->hash) {
#if 0
      if (strcmp(key, p->id) != 0) {
        fprintf(stderr, "Hash collision:\n1) %s\n2) %s\n", key, p->id);
      }
#endif
      assert(p->values);
      struct value* v = malloc(sizeof *v);
      assert(v);
      v->text = strdup(value);
      assert(v->text);
      v->next = p->values;
      p->values = v;
      goto end;
    }
  }
  if (bucket->length >= bucket->capacity) {
    bucket->capacity *= 2;
    bucket->keys = realloc(bucket->keys, bucket->capacity * sizeof bucket->keys[0]);
    assert(bucket->keys);
  }
  struct key* k = &bucket->keys[bucket->length];
  k->used_values = 0;
  k->id = strdup(key);
  assert(k->id);
  k->hash = hash((unsigned char*)k->id);
  k->values = malloc(sizeof *(k->values));
  assert(k->values);
  k->values->text = strdup(value);
  assert(k->values->text);
  k->values->next = 0;
  bucket->length += 1;
end:
  Pthread_mutex_unlock(&bucket->lock);
}

static void* mapper(void* arg) {
  for (;;) {
    Pthread_mutex_lock(&file_mutex);
    if (next_file >= argc) {
      Pthread_mutex_unlock(&file_mutex);
      break;
    }
    char* file = argv[next_file];
    ++next_file;
    Pthread_mutex_unlock(&file_mutex);
    map(file);
  }
  return 0;
}

char* get_next_value(char *key, int partition_number) {
  unsigned long key_hash = hash((unsigned char*)key);
  struct bucket* bucket = &buckets[partition_number];
  for (size_t i = 0; i < bucket->length; ++i) {
    struct key* k = &bucket->keys[i];
    if (key_hash == k->hash) {
      struct value* v = k->values;
      if (v) {
        k->values = v->next;
        v->next = k->used_values;
        k->used_values = v;
        return v->text;
      } else {
        return 0;
      }
    }
  }
  assert(0);
}

static int compare_keys(const void *p1, const void *p2) {
  struct key x1 = *((struct key*)p1);
  struct key x2 = *((struct key*)p2);
  return strcmp(x1.id, x2.id);
}

static void* reducer(void* arg) {
  size_t partition_number = (size_t) arg;
  struct bucket* bucket = &buckets[partition_number];

  qsort(bucket->keys, bucket->length, sizeof(bucket->keys[0]), compare_keys);

  for (size_t i = 0; i < bucket->length; ++i) {
    reduce(bucket->keys[i].id, get_next_value, partition_number);
  }
  return 0;
}

void MR_Run(int _argc, char *_argv[],
            Mapper _map, int num_mappers,
            Reducer _reduce, int _num_reducers,
            Partitioner _partition) {
  argc         = _argc;
  argv         = _argv;
  map          = _map;
  reduce       = _reduce;
  partition    = _partition;
  num_reducers = _num_reducers;

  assert(argc > 1);
  assert(argv);
  assert(map);
  assert(num_mappers > 0);
  assert(reduce);
  assert(num_reducers > 0);
  assert(partition);

  buckets = malloc(num_reducers * sizeof buckets[0]);
  assert(buckets);
  for (int i = 0; i < num_reducers; ++i) {
    Pthread_mutex_init(&buckets[i].lock, 0);
    buckets[i].capacity = 1024;
    buckets[i].length = 0;
    buckets[i].keys = malloc(buckets[i].capacity * sizeof buckets[i].keys[0]);
    assert(buckets[i].keys);
  }

  pthread_t mappers[num_mappers];
  for (int i = 0; i < num_mappers; ++i) {
    Pthread_create(&mappers[i], 0, mapper, 0);
  }
  for (int i = 0; i < num_mappers; ++i) {
    Pthread_join(mappers[i], 0);
  }

  pthread_t reducers[num_reducers];
  for (size_t i = 0; i < num_reducers; ++i) {
    Pthread_create(&reducers[i], 0, reducer, (void*) i);
  }
  for (int i = 0; i < num_reducers; ++i) {
    Pthread_join(reducers[i], 0);
  }
}
