#ifndef LAB_H
#define LAB_H

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SEMKEY 0x10
#define MSGKEY 0x20

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *arr;
};

struct child {
  pid_t pid;
  char *arg;
};

int create(const char *path,
  struct child *info);

int destroy(struct child *info);

void getstr(char *str, int n);

int main(void);

#endif /* LAB_H */
