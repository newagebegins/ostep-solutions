#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>

#if 0
#define Debug_print(...) printf(__VA_ARGS__)
#else
#define Debug_print(...)
#endif

#define Pthread_create(threadp, attrp, start_routinep, argp) assert(pthread_create(threadp, attrp, start_routinep, argp) == 0)
#define Pthread_join(thread, value_ptr) assert(pthread_join(thread, value_ptr) == 0)

#define Pthread_mutex_init(mutexp, attrp) assert(pthread_mutex_init(mutexp, attrp) == 0)
#define Pthread_cond_init(condp, attrp) assert(pthread_cond_init(condp, attrp) == 0)

#define Pthread_mutex_lock(mutexp) assert(pthread_mutex_lock(mutexp) == 0)
#define Pthread_mutex_unlock(mutexp) assert(pthread_mutex_unlock(mutexp) == 0)
#define Pthread_cond_wait(condp, mutexp) assert(pthread_cond_wait(condp, mutexp) == 0)
#define Pthread_cond_signal(condp) assert(pthread_cond_signal(condp) == 0)

#define Write(fildes, bufp, nbyte) assert(write(fildes, bufp, nbyte) != -1)

#define NUM_ZIP_THREADS 3
#define NUM_CHUNKS (NUM_ZIP_THREADS * 2)
#define MAX_CHUNK_SIZE 1024*1024
#define NUM_ZIPPED_CHUNKS 16

struct __attribute__((__packed__)) run {
  int length;
  unsigned char byte;
};

struct chunk {
  unsigned char* data;
  size_t size;
  size_t index;
  bool finish;
};

struct zipped_chunk {
  pthread_mutex_t mutex;
  pthread_cond_t cv;
  size_t index;
  struct run runs[MAX_CHUNK_SIZE];
  size_t num_runs;
  bool ready;
  bool finish;
};

struct chunk chunks[NUM_CHUNKS] = {0};
pthread_mutex_t chunks_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t fill_cv = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty_cv = PTHREAD_COND_INITIALIZER;
size_t put_index = 0;
size_t get_index = 0;
size_t num_filled = 0;

struct zipped_chunk zipped_chunks[NUM_ZIPPED_CHUNKS] = {0};

void* zip(void* arg) {
  size_t id = (size_t) arg;
  while (1) {
    Pthread_mutex_lock(&chunks_mutex);
    while (num_filled == 0) {
      Debug_print("zip %zu: wait for fill\n", id);
      Pthread_cond_wait(&fill_cv, &chunks_mutex);
    }
    struct chunk *chunk = &chunks[get_index];
  
    if (chunk->finish) {
      Debug_print("zip %zu: finish\n", id);
      Pthread_mutex_unlock(&chunks_mutex);
      return 0;
    }

    Debug_print("zip %zu: process chunk %zu\n", id, chunk->index);

    struct zipped_chunk* zc = &zipped_chunks[chunk->index % NUM_ZIPPED_CHUNKS];
    Pthread_mutex_lock(&zc->mutex);
    while (!(zc->index == chunk->index && !zc->ready)) {
      Debug_print("zip %zu: wait for zipped chunk %zu\n", id, chunk->index % NUM_ZIPPED_CHUNKS);
      Pthread_cond_wait(&zc->cv, &zc->mutex);
    }

    Debug_print("zip %zu: write zipped chunk %zu\n", id, chunk->index % NUM_ZIPPED_CHUNKS);

    zc->num_runs = 0;
    int n = 1;
    unsigned char* end = chunk->data + chunk->size;
    unsigned char* p = chunk->data;
    unsigned char prev = *p;
    ++p;

    for (; p < end; ++p) {
      if (*p == prev) {
        ++n;
      } else {
        zc->runs[zc->num_runs].byte = prev;
        zc->runs[zc->num_runs].length = n;
        ++zc->num_runs;
        n = 1;
      }
      prev = *p;
    }

    zc->runs[zc->num_runs].byte = prev;
    zc->runs[zc->num_runs].length = n;
    ++zc->num_runs;

    zc->ready = true;
    Pthread_cond_signal(&zc->cv);
    Pthread_mutex_unlock(&zc->mutex);

    get_index = (get_index + 1) % NUM_CHUNKS;
    --num_filled;
    Pthread_cond_signal(&empty_cv);
    Pthread_mutex_unlock(&chunks_mutex);
  }
}

void* print(void* arg) {
  size_t next = 0;
  struct run prev_last_run = {0};

  while (1) {
    struct zipped_chunk* chunk = &zipped_chunks[next % NUM_ZIPPED_CHUNKS];

    Pthread_mutex_lock(&chunk->mutex);
    while (!chunk->ready) {
      Debug_print("print: wait for zipped chunk %zu\n", next % NUM_ZIPPED_CHUNKS);
      Pthread_cond_wait(&chunk->cv, &chunk->mutex);
    }

    if (chunk->finish) {
      Debug_print("print: finish at zipped chunk %zu\n", next % NUM_ZIPPED_CHUNKS);
      Write(STDOUT_FILENO, &prev_last_run, sizeof(struct run));
      Pthread_mutex_unlock(&chunk->mutex);
      return 0;
    }

    Debug_print("print: print zipped chunk %zu\n", next % NUM_ZIPPED_CHUNKS);

    if (next > 0) {
      if (prev_last_run.byte == chunk->runs[0].byte) {
        chunk->runs[0].length += prev_last_run.length;
      } else {
        Write(STDOUT_FILENO, &prev_last_run, sizeof(struct run));
      }
    }
    prev_last_run = chunk->runs[chunk->num_runs - 1];

    if (chunk->num_runs > 1) {
      Write(STDOUT_FILENO, chunk->runs, (chunk->num_runs - 1) * sizeof(struct run));
    }

    chunk->index += NUM_ZIPPED_CHUNKS;
    chunk->ready = false;
    Pthread_cond_signal(&chunk->cv);
    Pthread_mutex_unlock(&chunk->mutex);

    ++next;
  }
}

int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("wzip: file1 [file2 ...]\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < NUM_ZIPPED_CHUNKS; ++i) {
    zipped_chunks[i].index = i;
    Pthread_mutex_init(&zipped_chunks[i].mutex, 0);
    Pthread_cond_init(&zipped_chunks[i].cv, 0);
  }

  pthread_t zip_threads[NUM_ZIP_THREADS];
  pthread_t print_thread;

  for (size_t i = 0; i < NUM_ZIP_THREADS; ++i) {
    Pthread_create(&zip_threads[i], 0, zip, (void *) i);
  }
  Pthread_create(&print_thread, 0, print, 0);

  size_t chunk_index = 0;

  for (size_t i = 1; i < argc; ++i) {
    int fd = open(argv[i], O_RDONLY);
    if (fd < 0) {
      printf("wzip: cannot open file\n");
      exit(EXIT_FAILURE);
    }

    Debug_print("main: opened %s\n", argv[i]);

    struct stat st;
    if (fstat(fd, &st) != 0) {
      perror("fstat");
      exit(EXIT_FAILURE);
    }

    unsigned char* start = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (start == MAP_FAILED) {
      perror("mmap");
      exit(EXIT_FAILURE);
    }

    size_t size_left = st.st_size;
    unsigned char* p = start;

    while (size_left) {
      Pthread_mutex_lock(&chunks_mutex);
      while (num_filled == NUM_CHUNKS) {
        Debug_print("main: wait for empty chunk\n");
        Pthread_cond_wait(&empty_cv, &chunks_mutex);
      }

      Debug_print("main: fill chunks[%zu].index = %zu\n", put_index, chunk_index);

      chunks[put_index].data = p;
      chunks[put_index].index = chunk_index;

      if (size_left <= MAX_CHUNK_SIZE) {
        chunks[put_index].size = size_left;
        size_left = 0;
      } else {
        chunks[put_index].size = MAX_CHUNK_SIZE;
        size_left -= MAX_CHUNK_SIZE;
      }

      p += MAX_CHUNK_SIZE;
      ++chunk_index;

      put_index = (put_index + 1) % NUM_CHUNKS;
      ++num_filled;
      Pthread_cond_signal(&fill_cv);
      Pthread_mutex_unlock(&chunks_mutex);
    }

    Pthread_mutex_lock(&chunks_mutex);
    while (num_filled > 0) {
      Debug_print("main: wait for num_filled == 0\n");
      Pthread_cond_wait(&empty_cv, &chunks_mutex);
    }
    Debug_print("main: close %s\n", argv[i]);
    if (munmap(start, st.st_size) != 0) {
      perror("munmap");
      exit(EXIT_FAILURE);
    }
    if (close(fd) != 0) {
      perror("close");
      exit(EXIT_FAILURE);
    }
    Pthread_mutex_unlock(&chunks_mutex);
  }

  for (size_t i = 0; i < NUM_ZIP_THREADS; ++i) {
    Pthread_mutex_lock(&chunks_mutex);
    while (num_filled == NUM_CHUNKS) {
      Debug_print("main: finish, wait for empty chunk\n");
      Pthread_cond_wait(&empty_cv, &chunks_mutex);
    }

    Debug_print("main: finish chunks[%zu]\n", put_index);

    chunks[put_index].finish = true;

    put_index = (put_index + 1) % NUM_CHUNKS;
    ++num_filled;
    Pthread_cond_signal(&fill_cv);
    Pthread_mutex_unlock(&chunks_mutex);
  }

  for (size_t i = 0; i < NUM_ZIP_THREADS; ++i) {
    Pthread_join(zip_threads[i], 0);
  }

  for (size_t i = 0; i < NUM_ZIPPED_CHUNKS; ++i) {
    struct zipped_chunk* chunk = &zipped_chunks[i];
    Pthread_mutex_lock(&chunk->mutex);
    while (chunk->ready) {
      Debug_print("main: wait for zipped_chunks[%zu]\n", i);
      Pthread_cond_wait(&chunk->cv, &chunk->mutex);
    }
    Debug_print("main: finish zipped_chunks[%zu]\n", i);
    chunk->finish = true;
    chunk->ready = true;
    Pthread_cond_signal(&chunk->cv);
    Pthread_mutex_unlock(&chunk->mutex);
  }

  Pthread_join(print_thread, 0);
}
