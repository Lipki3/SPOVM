#include "lab.h"

int create(const char *path,
  struct child *info)
{
  if (!path || !info)
    return -1;

  info->pid = fork();

  if (!info->pid)
    if (execl(path, info->arg, (char *)NULL) == -1) {
      fprintf(stderr, "\033[91;1merror: "
        "\033[0;1m%s: \033[0mcouldn\'t create "
        "a process\n", info->arg);

      exit(EXIT_FAILURE);
    }


  printf("\n\033[0;1m%12s: \033[0m%d\n", "pid", info->pid);
  printf("\033[92;1m%12s: \033[0m%s\n", "executing", info->arg);

  return 0;
}

int destroy(struct child *info)
{
  if (!info)
    return -1;

  printf("\n\033[0;1m%12s: \033[0m%d\n", "pid", info->pid);
  printf("\033[93;1m%12s: \033[0m%s\n", "killing", info->arg);

  return kill(info->pid, SIGABRT);
}
