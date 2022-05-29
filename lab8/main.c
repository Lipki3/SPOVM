#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include "ringbuffer.h"


static struct ring_buffer * temp_ring_buffer = NULL;
static pthread_mutex_t mutex;
static int mutex_status = 0;
static int cons_status[100];
static int pros_status[100];
static int status_quit = 0;


void * thread_cons_func(void * arg);
void * thread_pros_func(void * arg);


int main(int argc, char ** argv) {
    temp_ring_buffer = ring_buffer_init(100);
    pthread_t thread_cons[100], thread_pros[100];
    int num_pros = 0;
    int num_cons = 0;


    int get_event = ' ';
    while((get_event = getchar()) != 'q') {
        if(get_event == (int)'-') { 
            pthread_create(&thread_pros[num_pros], NULL, thread_pros_func, NULL);
            pros_status[num_pros] = 1;
            num_pros++;
            printf("Pros number %d was created.\n", num_pros);
        }
        if(get_event == (int)'=') {         
            pthread_create(&thread_cons[num_cons], NULL, thread_cons_func, NULL);
            cons_status[num_cons]= 1;
            num_cons++;
            printf("Cons number %d was created.\n", num_cons);
        }
        if (num_pros >0) {
        if(get_event == (int)'_') {         
            pthread_cancel(thread_pros[num_pros-1]);
            printf("Pros number %d was deleted.\n", num_pros);
            pros_status[num_pros] = 0;
            num_pros--;
        }  
        }
        if(num_cons>0){
        if(get_event == (int)'+') { 
            pthread_cancel(thread_cons[num_cons-1]);
            printf("Cons number %d was deleted.\n", num_cons);
            cons_status[num_cons] = 0;
            num_cons--;
        } 
        }
        if(get_event == (int)'q')
            break;
    }
}

void * thread_cons_func(void * arg) {
    while(cons_status) {
        pthread_mutex_lock(&mutex);
        int read_result = read_message_from_buffer(temp_ring_buffer);
        printf("\n\n");
        pthread_mutex_unlock(&mutex);
        usleep(1000000);
        if(read_result == -1) {
            while(temp_ring_buffer -> in == temp_ring_buffer -> out) {
                continue;             
                status_quit = 0;
            }
           printf("\n");
        } else status_quit = 1;
    }
}

void * thread_pros_func(void * arg) {
    while(pros_status) {
        pthread_mutex_lock(&mutex);
        write_message_to_buffer(temp_ring_buffer);
        pthread_mutex_unlock(&mutex);
        usleep(1500000);
    }
}
