#include "cons.h"

int iswork = 1;

void handle(int signum)
{
  iswork = 0;
}

int main(int argc, char *argv[])
{

  signal(SIGTERM, handle);

  int semid;

  if ((semid = semget(SEMKEY, 3, 0666)) == -1) {
    fprintf(stderr, "\033[0;1m%s: \033[91;1merror: "
      "\033[0mcouldn\'t get semaphores\n", argv[0]);

    return 1;
  }

  int msgid;

  if ((msgid = msgget(MSGKEY, 0666)) == -1) {
    fprintf(stderr, "\033[0;1m%s: \033[91;1merror: "
      "\033[0mcouldn\'t get message queue\n", argv[0]);

    return 1;
  }

  /* A time to sleep. */
  struct timespec rqtp = { 5, 0 };

  /* An array of structures
   * representing semaphores
   * operations. */
  struct sembuf sops[2];

  struct msgbuf mbuf;

  while (iswork) {
    /* Sleeping until
     * interruption by
     * the signal. */
    nanosleep(&rqtp, NULL);

    /* Locking 'izv' semaphore. */
    sops[0].sem_num =  0;
    sops[0].sem_op  = -1;
    sops[0].sem_flg =  0;

    /* Locking 'mutex' semaphore. */
    sops[1].sem_num =  2;
    sops[1].sem_op  = -1;
    sops[1].sem_flg =  0;

    /* Operating of semaphores... */
    semop(semid, sops, 2);

    /* Working with message queue... */
    if (msgrcv(msgid, &mbuf, MSGSIZ, 1, MSG_NOERROR | IPC_NOWAIT) >= 0) {
      sops[0].sem_num = 4;
      sops[0].sem_op  = 1;
      sops[0].sem_flg = 0;

      semop(semid, sops, 1);

      printf("\n\033[0;1m%12d: \033[0m%s\n", getpid(), argv[0]);
      printf("\033[94;1m%12s: \033[0m%s "
        "\033[90;1m[%d]\033[0m\n", "received", mbuf.mtext,
        semctl(semid, 4, GETVAL));
    }

    /* Unlocking 'mutex' semaphore. */
    sops[0].sem_num = 2;
    sops[0].sem_op  = 1;
    sops[0].sem_flg = 0;

    /* Unlocking 'zap' semaphore. */
    sops[1].sem_num = 1;
    sops[1].sem_op  = 1;
    sops[1].sem_flg = 0;

    /* Operating of semaphores... */
    semop(semid, sops, 2);
  }

  return 0;
}
