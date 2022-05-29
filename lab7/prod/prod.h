#ifndef PROD_H
#define PROD_H

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define SEMKEY 0x10
#define MSGKEY 0x20

#define MSGSIZ 16


struct msgbuf {
  long mtype;
  char mtext[MSGSIZ];
};

int main(int argc, char *argv[]);

void strfill(char *str, size_t n);

#endif /* PROD_H */
