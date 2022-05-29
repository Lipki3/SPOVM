#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>



static const char * string   = "THREAD_";
static pthread_mutex_t mutex;

static int threads = 0;
static int q = 0;

void * print_string(char * str, int num) {
    pthread_mutex_lock(&mutex);
    for(int j = 0; j < 7; j++) {
        printf("%c", str[j]);
        usleep(50000);
        fflush(stdout);
    }
    printf("%d\n", num);
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

void* option(void * arg) {
   char c;
   do  {
    fflush(stdin);
       scanf("%c",&c);
     if(c == '+') {
     	threads++;
     	}
    
     if(c == '-') 
     {}
     	
      
     if(c == 'k') {
     threads = 0;
     printf("All threads has been deleted\n"); 
     }
    } while (c != 'q');
     if(c=='q') {
     q = 1;
     return 0;
     }
}

int main(int argc, char ** argv) {
    pthread_t threadss[100], main_thread;
   char * str = "THREAD_";
    pthread_create(&main_thread, NULL, option, NULL);
    while(1) {
    if (q == 1) break;
     
    
    sleep(2);
	
        for (int i = 0; i<threads; i++) {
        pthread_create(&threadss[i],  NULL, print_string(str,i), (void *) string);
     }
       
    }
    return 0;
}



