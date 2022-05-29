#include "lab.h"

#define ARRCAP 10

#define CMDSIZ 16

static struct {
  struct child ptr[ARRCAP];
  size_t len;
} prods, conses;

union semun arg;

int main(void)
{

  int msgid = msgget(MSGKEY, IPC_CREAT | 0666);

  int semid = semget(SEMKEY, 4,IPC_CREAT | 0666);

  /* Initializing 'izv'
   * semaphore. */
  arg.val = 0;
  semctl(semid, 0, SETVAL, arg);

  /* Initializing 'zap'
   * semaphore. */
  arg.val = ARRCAP;
  semctl(semid, 1, SETVAL, arg);

  /* Initializing 'mutex'
   * semaphore. */
  arg.val = 1;
  semctl(semid, 2, SETVAL, arg);

  /* Initializing 'msg'
   * semaphore. */
  arg.val = 0;
  semctl(semid, 3, SETVAL, arg);



  char cmd[CMDSIZ];
  char prev[CMDSIZ] = { '\0' };

  size_t i;

  prods.len = conses.len = 0;

  while (1) {
    getstr(cmd, CMDSIZ);

    if (!*cmd) {
      if (!*prev) {
        fprintf(stderr, "\033[91;1merror: "
          "\033[0mno previous command\n");
        continue;
      }

      strcpy(cmd, prev);
    }

    int ind = 0;

    if (strcmp(cmd, "q") == 0)
      break;

    switch (cmd[ind++]) {
    case '+':
      if (!cmd[ind]) {
        fprintf(stderr, "\033[91;1merror: "
          "\033[0;1m+: \033[0mno type specified\n");
        break;
      }

      if (strcmp(cmd + ind, "p")==0) {
        if (prods.len >= ARRCAP) {
          fprintf(stderr, "\033[91;1merror: "
            "\033[0;1m+: \033[0mthe stack is full\n");
          break;
        }

        prods.ptr[prods.len].arg = (char *)malloc(CMDSIZ);
        sprintf(prods.ptr[prods.len].arg, "prod-%1zu", prods.len);

        if (!create("../prod/prod", &prods.ptr[prods.len]))
          ++prods.len;
      } else if (strcmp(cmd + ind, "c")==0) {
        if (conses.len >= ARRCAP) {
          fprintf(stderr, "\033[91;1merror: "
            "\033[0;1m+: \033[0mthe stack is full\n");
          break;
        }

        conses.ptr[conses.len].arg = (char *)malloc(CMDSIZ);
        sprintf(conses.ptr[conses.len].arg, "cons-%1zu", conses.len);

        if (!create("../cons/cons", &conses.ptr[conses.len]))
          ++conses.len;
      } else {
        fprintf(stderr, "\033[91;1merror: "
          "\033[0;1m+: \033[0munknown type -- "
          "\033[0;1m\'%s\'\033[0m\n", cmd + ind);
        break;
      }

      break;
    case '-':
      if (!cmd[ind]) {
        fprintf(stderr, "\033[91;1merror: "
          "\033[0;1m-: \033[0mno type specified\n");
        break;
      }

      if (strcmp(cmd + ind, "p")==0 ) {
        if (prods.len == 0) {
          fprintf(stderr, "\033[91;1merror: "
            "\033[0;1m-: \033[0mthe stack is empty\n");
          break;
        }

        destroy(&prods.ptr[--prods.len]);
        free(prods.ptr[prods.len].arg);
      } else if (strcmp(cmd + ind, "c") == 0) {

        if (conses.len == 0) {
          fprintf(stderr, "\033[91;1merror: "
            "\033[0;1m-: \033[0mthe stack is empty\n");
          break;
        }

        destroy(&conses.ptr[--conses.len]);
        free(conses.ptr[conses.len].arg);
      } else {
        fprintf(stderr, "\033[91;1merror: "
          "\033[0;1m-: \033[0munknown type -- "
          "\033[0;1m\'%s\'\033[0m\n", cmd + ind);
        break;
      }

      break;
    case 'k':

      for (i = 0; i < prods.len; ++i) {
        destroy(&prods.ptr[i]);
        free(prods.ptr[i].arg);
      }

      for (i = 0; i < conses.len; ++i) {
        destroy(&conses.ptr[i]);
        free(conses.ptr[i].arg);
      }

      prods.len = conses.len = 0;

      break;
    default:
      fprintf(stderr, "\033[91;1merror: "
        "\033[0munknown command -- "
        "\033[0;1m\'%s\'\033[0m\n", cmd);
    }

    strcpy(prev, cmd);
  }

  for (i = 0; i < prods.len; ++i) {
    destroy(&prods.ptr[i]);
    free(prods.ptr[i].arg);
  }

  for (i = 0; i < conses.len; ++i) {
    destroy(&conses.ptr[i]);
    free(conses.ptr[i].arg);
  }

  msgctl(msgid, IPC_RMID, NULL);
  semctl(semid, 3, IPC_RMID);

  return 0;
}
