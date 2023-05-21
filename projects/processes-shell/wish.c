#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

// Command examples:
// exit
// cd foo
// path
// path /bin /usr/bin
// cmd1 arg1 arg2 > foo&cmd2 >bar & cmd3

void print_error() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

char* get_token(char** line, size_t* token_len) {
  char* token = 0;
  *token_len = 0;
  while (**line) {
    if (**line == ' ' || **line == '\t' || **line == '\n') {
      ++(*line);
      if (token) {
        break;
      }
    } else if (**line == '>' || **line == '&') {
      if (!token) {
        token = *line;
        *token_len = 1;
        ++(*line);
      }
      break;
    } else {
      if (!token) {
        token = *line;
      }
      ++(*token_len);
      ++(*line);
    }
  }
  return token;
}

#define PATHS_CAP 16
char* paths[PATHS_CAP] = {0};
size_t paths_len = 0;

#define MAX_PATH_LEN 128

char* get_full_path(char* prog) {
  for (size_t i = 0; i < paths_len; ++i) {
    char* full_path = calloc(MAX_PATH_LEN, 1);
    size_t n = 0;

    n = strlcat(full_path, paths[i], MAX_PATH_LEN);
    assert(n < MAX_PATH_LEN);

    n = strlcat(full_path, "/", MAX_PATH_LEN);
    assert(n < MAX_PATH_LEN);

    n = strlcat(full_path, prog, MAX_PATH_LEN);
    assert(n < MAX_PATH_LEN);

    if (access(full_path, X_OK) == 0) {
      return full_path;
    }
  }
  return 0;
}

#define ARGV_CAP 32

pid_t exec_command(char* argv[ARGV_CAP], char* redirect_to) {
  char* full_path = get_full_path(argv[0]);
  if (!full_path) {
    return -1;
  }
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork failed");
    exit(1);
  } else if (pid == 0) {
    if (redirect_to) {
      close(STDOUT_FILENO);
      open(redirect_to, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    }
    execv(full_path, argv);
    exit(0);
  } else {
    free(full_path);
    return pid;
  }
}

void exec_commands(char* line) {
  size_t argv_len = 0;
  char* argv[ARGV_CAP] = {0};
  char* token = 0;
  size_t token_len = 0;
  bool is_redirect = false;
  char* redirect_to = 0;
  bool has_running_children = false;
  while ((token = get_token(&line, &token_len))) {
    if (strncmp(token, ">", token_len) == 0) {
      if (argv_len == 0 || is_redirect) {
        print_error();
        goto cleanup;
      } else {
        is_redirect = true;
      }
    } else if (strncmp(token, "&", token_len) == 0) {
      if (argv_len > 0) {
        if (exec_command(argv, redirect_to) > 0) {
          has_running_children = true;
        } else {
          print_error();
        }
        for (size_t i = 0; i < argv_len; ++i) {
          free(argv[i]);
          argv[i] = 0;
        }
        argv_len = 0;
        if (redirect_to) {
          free(redirect_to);
          redirect_to = 0;
          is_redirect = false;
        }
      }
    } else {
      if (is_redirect) {
        if (redirect_to) {
          print_error();
          goto cleanup;
        } else {
          redirect_to = strndup(token, token_len);
        }
      } else {
        // +1, because last element must be 0
        if (argv_len + 1 >= ARGV_CAP) {
          fprintf(stderr, "too many arguments\n");
          print_error();
          goto cleanup;
        }
        argv[argv_len] = strndup(token, token_len);
        assert(argv[argv_len]);
        argv_len++;
      }
    }
  }
  if (argv_len > 0) {
    if (is_redirect && !redirect_to) {
      print_error();
    } else {
      if (exec_command(argv, redirect_to) > 0) {
        has_running_children = true;
      } else {
        print_error();
      }
    }
  }
cleanup:
  for (size_t i = 0; i < argv_len; ++i) {
    free(argv[i]);
  }
  free(redirect_to);
  if (has_running_children) {
    while (wait(0) > 0);
  }
}

void do_exit(char* line) {
  size_t token_len = 0;
  char* token = get_token(&line, &token_len);
  if (token) {
    print_error();
  } else {
    exit(0);
  }
}

void do_cd(char* line) {
  size_t token_len = 0;
  char* token = get_token(&line, &token_len);
  if (token) {
    size_t token_len2 = 0;
    char* token2 = get_token(&line, &token_len2);
    if (token2) {
      print_error();
    } else {
      char* dir = strndup(token, token_len);
      if (chdir(dir) == -1) {
        print_error();
      }
      free(dir);
    }
  } else {
    print_error();
  }
}

void set_path(char* line) {
  for (size_t i = 0; i < paths_len; ++i) {
    free(paths[i]);
    paths[i] = 0;
  }
  paths_len = 0;

  char* token = 0;
  size_t token_len = 0;
  while ((token = get_token(&line, &token_len))) {
    paths[paths_len] = strndup(token, token_len);
    ++paths_len;
  }
}

void process_line(char* line) {
  char* rest_line = line;
  size_t token_len = 0;
  char* token = get_token(&rest_line, &token_len);
  if (token) {
    if (strncmp(token, "exit", token_len) == 0) {
      do_exit(rest_line);
      return;
    }
    if (strncmp(token, "cd", token_len) == 0) {
      do_cd(rest_line);
      return;
    }
    if (strncmp(token, "path", token_len) == 0) {
      set_path(rest_line);
      return;
    }
    if (strncmp(token, ">", token_len) == 0) {
      print_error();
      return;
    }
    exec_commands(line);
  }
}

int main(int argc, char* argv[argc+1]) {
  if (argc > 2) {
    print_error();
    exit(1);
  }

  FILE* file = argc == 2 ? fopen(argv[1], "r") : stdin;
  if (!file) {
    print_error();
    exit(1);
  }

  set_path("/bin");

  char* line = 0;
  for (;;) {
    if (file == stdin) {
      printf("wish1> ");
    }
    size_t linecap = 0;
    ssize_t linelen = getline(&line, &linecap, file);
    if (linelen == -1) {
      break;
    }
    process_line(line);
  }
  free(line);
}
