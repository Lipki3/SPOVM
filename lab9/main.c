#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
struct user{
    char name[20];
    char message[100];
};


static int fd = 0;
struct user chat; 


void *write_m(void * arg);
bool uniqe(struct user * user1, struct user * user2);

static pthread_mutex_t mutex_write, mutex_read;

int main() {
    system("clear");

    pthread_t thread;
    struct user empty_user = {""};

    fd = open("buffer", O_RDWR);  
    if (fd < 0) {
        perror("File opened error..");
        exit(EXIT_FAILURE);
    }

    printf("Enter name: ");
    gets(chat.name);

    ftruncate (fd, sizeof(struct user)); //обрезка файла до нужного размера
    void * shared_memory = NULL;
    shared_memory = mmap(0, sizeof(struct user), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //
    pthread_create(&thread, NULL, write_m, shared_memory); 
    struct user temp_user_r;
    while(true) {

        memcpy(&temp_user_r, shared_memory, sizeof(struct user)); 
        if(uniqe((struct user *)&chat, (struct user *)&temp_user_r) == true) continue;
        while(!strcmp(temp_user_r.message, "")){
            memcpy(&temp_user_r, shed_memory, sizeof(struct user));
        }
        printf("\n%s: %s\n", temp_user_r.name, temp_user_r.message);  
        memcpy(shared_memory, &empty_user, sizeof(struct user));
    }
}
 

void *write_m(void * arg) {
    while(true) {
     pthread_mutex_lock(&mutex_write);
     fflush(stdin);
     gets(chat.message);
     memcpy(arg, &chat, sizeof(struct user));
     pthread_mutex_unlock(&mutex_write);
    }
}



bool uniqe(struct user * user1, struct user * user2) {
    if(!strcmp(user2 -> name, "")) return true;
    if(strcmp(user1 -> name, user2 -> name)) return false;
    return true;
}
