#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "mapreduce.h"

#define Pthread_create(threadp, attrp, start_routinep, argp) assert(pthread_create(threadp, attrp, start_routinep, argp) == 0)
#define Pthread_join(thread, value_ptr) assert(pthread_join(thread, value_ptr) == 0)

#define Pthread_mutex_init(mutexp, attrp) assert(pthread_mutex_init(mutexp, attrp) == 0)
#define Pthread_mutex_lock(mutexp) assert(pthread_mutex_lock(mutexp) == 0)
#define Pthread_mutex_unlock(mutexp) assert(pthread_mutex_unlock(mutexp) == 0)

#define NUM_BINS (1024)

struct value {
  char* text;
  struct value* next;
};

struct key {
  char* id;
  struct value* values;
  struct value* used_values;
  struct key* next;
};

struct bin {
  pthread_mutex_t lock;
  struct key* keys;
  size_t num_keys;
};

struct bucket {
  struct bin bins[NUM_BINS];
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

void MR_Emit(char *key, char *value) {
  unsigned long partition_number = partition(key, num_reducers);
  unsigned long bin_number = MR_DefaultHashPartition(key, NUM_BINS);
  struct bucket* bucket = &buckets[partition_number];
  struct bin* bin = &bucket->bins[bin_number];
  Pthread_mutex_lock(&bin->lock);
  for (struct key* p = bin->keys; p; p = p->next) {
    if (strcmp(key, p->id) == 0) {
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
  struct key* k = malloc(sizeof *k);
  assert(k);
  k->used_values = 0;
  k->id = strdup(key);
  assert(k->id);
  k->values = malloc(sizeof *(k->values));
  assert(k->values);
  k->values->text = strdup(value);
  assert(k->values->text);
  k->values->next = 0;
  k->next = bin->keys;
  bin->keys = k;
  bin->num_keys += 1;
end:
  Pthread_mutex_unlock(&bin->lock);
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
  unsigned long bin_number = MR_DefaultHashPartition(key, NUM_BINS);
  struct bucket* bucket = &buckets[partition_number];
  struct bin* bin = &bucket->bins[bin_number];
  for (struct key* k = bin->keys; k; k = k->next) {
    if (strcmp(key, k->id) == 0) {
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
  struct key* x1 = *((struct key**)p1);
  struct key* x2 = *((struct key**)p2);
  return strcmp(x1->id, x2->id);
}

static void* reducer(void* arg) {
  size_t partition_number = (size_t) arg;
  struct bucket* bucket = &buckets[partition_number];

  // Sort keys

  size_t num_keys_total = 0;
  for (size_t i = 0; i < NUM_BINS; ++i) {
    struct bin* bin = &bucket->bins[i];
    num_keys_total += bin->num_keys;
  }

  struct key** sorted_keys = malloc(num_keys_total * sizeof sorted_keys[0]);
  assert(sorted_keys);

  size_t next_key = 0;

  for (size_t i = 0; i < NUM_BINS; ++i) {
    struct bin* bin = &bucket->bins[i];
    for (struct key* k = bin->keys; k; k = k->next) {
      sorted_keys[next_key] = k;
      ++next_key;
    }
  }

  assert(next_key == num_keys_total);

  qsort(sorted_keys, num_keys_total, sizeof sorted_keys[0], compare_keys);

  for (size_t i = 0; i < num_keys_total; ++i) {
    struct key* k = sorted_keys[i];
    reduce(k->id, get_next_value, partition_number);
  }

  free(sorted_keys);

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

  buckets = calloc(num_reducers, sizeof buckets[0]);
  assert(buckets);
  for (int i = 0; i < num_reducers; ++i) {
    for (int j = 0; j < NUM_BINS; ++j) {
      Pthread_mutex_init(&buckets[i].bins[j].lock, 0);
    }
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
