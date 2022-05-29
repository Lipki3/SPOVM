#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int number_of_different_pares[4];
sig_atomic_t signal_from_alarm;
bool out;

struct symbols{
    char a, b;
} memory;


void myAlarm(int sig){
    signal_from_alarm = 1;

    if (memory.a == '0' && memory.b == '0')
        number_of_different_pares[0]++;
    else if (memory.a == '0' && memory.b == '1')
        number_of_different_pares[1]++;
    else if (memory.a == '1' && memory.b == '0')
        number_of_different_pares[2]++;
    else if (memory.a == '1' && memory.b == '1')
        number_of_different_pares[3]++;
}

void ban(int sig){
    out = false;
}

void disban(int sig){
    out = true;
}

void immediate_stat(int sig){
    stat();
}

void stat(){
    printf("pid: %d\n"
           "ppid: %d\n"
           "00: %d\n"
           "01: %d\n"
           "10: %d\n"
           "11: %d\n",
           getpid(), getppid(), number_of_different_pares[0], number_of_different_pares[1],
           number_of_different_pares[2], number_of_different_pares[3]);

}

int main()
{

    struct symbols zeroes = {'0', '0'}, ones = {'1', '1'};

    int number_of_iterations = 0;
struct sigaction action, oldaction;
action.sa_handler = myAlarm; 

    sigaction(SIGALRM, &action, &oldaction);
    action.sa_handler = ban; 
    sigaction(SIGUSR1, &action,  &oldaction);
action.sa_handler = disban; 
    sigaction(SIGUSR2, &action,  &oldaction);
    action.sa_handler = immediate_stat; 
    sigaction(SIGINT, &action,  &oldaction);
    while (1)
    {
        signal_from_alarm = 0;
        if (number_of_iterations >= 10 && out)
        {
            stat();
            number_of_iterations = 0;
        }

        alarm(1);
        usleep(9999);

        number_of_iterations++;

        while (!signal_from_alarm)
        {
            memory = zeroes;
            nanosleep(0, 22);
            memory = ones;
        }
    }
    return 0;
}


