#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "mapreduce.h"

#define Pthread_create(threadp, attrp, start_routinep, argp) assert(pthread_create(threadp, attrp, start_routinep, argp) == 0)
#define Pthread_join(thread, value_ptr) assert(pthread_join(thread, value_ptr) == 0)

#define Pthread_mutex_lock(mutexp) assert(pthread_mutex_lock(mutexp) == 0)
#define Pthread_mutex_unlock(mutexp) assert(pthread_mutex_unlock(mutexp) == 0)

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

static int argc = 0;
static char** argv = 0;
static Mapper map = 0;
static Reducer reduce = 0;
static Partitioner partition = 0;
static int num_reducers = 0;

static pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
static size_t next_file = 1;

static struct key** buckets = 0;

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
  unsigned long hash = 5381;
  int c;
  while ((c = *key++) != '\0')
    hash = hash * 33 + c;
  return hash % num_partitions;
}

void MR_Emit(char *key, char *value) {
  unsigned long partition_number = partition(key, num_reducers);
  if (buckets[partition_number]) {
    struct key* p = buckets[partition_number];
    struct key* prev_key = 0;
    while (p) {
      int cmp = strcmp(key, p->id);
      if (cmp == 0) {
        assert(p->values);
        struct value* v = malloc(sizeof *v);
        assert(v);
        v->text = strdup(value);
        assert(v->text);
        v->next = p->values;
        p->values = v;
        return;
      } else if (cmp < 0) {
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
        if (prev_key) {
          k->next = prev_key->next;
          prev_key->next = k;
        } else {
          k->next = buckets[partition_number];
          buckets[partition_number] = k;
        }
        return;
      }
      prev_key = p;
      p = p->next;
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
    k->next = prev_key->next; // 0
    prev_key->next = k;
  } else {
    struct key* k = malloc(sizeof *k);
    assert(k);
    k->used_values = 0;
    k->id = strdup(key);
    assert(k->id);
    k->values = malloc(sizeof k->values[0]);
    assert(k->values);
    k->values->text = strdup(value);
    assert(k->values->text);
    k->values->next = 0;
    k->next = 0;
    buckets[partition_number] = k;
  }
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
  for (struct key* k = buckets[partition_number]; k; k = k->next) {
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

static void* reducer(void* arg) {
  size_t partition_number = (size_t) arg;
  for (struct key* k = buckets[partition_number]; k; k = k->next) {
    reduce(k->id, get_next_value, partition_number);
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

  buckets = calloc(num_reducers, sizeof buckets[0]);
  assert(buckets);

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
