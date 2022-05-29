#include <stdlib.h>
#include <time.h>


struct ring_buffer { 
    unsigned char ** messages;
    int buffer_size;
    int in;
    int out;
} default_buffer;

unsigned char *create_message();
void print_message(const unsigned char *message);

struct ring_buffer *ring_buffer_init(unsigned int size) {

    if(size == 0) return NULL;
    struct ring_buffer * temp_ring_buffer = malloc(sizeof(default_buffer));
    if(temp_ring_buffer == NULL) return NULL;

    temp_ring_buffer -> buffer_size   = size;
    temp_ring_buffer -> messages      = malloc(sizeof(unsigned char *) * temp_ring_buffer -> buffer_size);
    for(int i = 0; i < temp_ring_buffer -> buffer_size; i++) {

        temp_ring_buffer -> messages[i] = NULL;

    }
    temp_ring_buffer -> in = 0;
    temp_ring_buffer -> out = 0;
    return temp_ring_buffer;
}

int write_message_to_buffer(struct ring_buffer * buffer_to_write) {

    if(buffer_to_write == NULL) {
        printf("BUFFER DIDN'T CREATE\n"); 
        return -1;
    }
    if((buffer_to_write -> in - buffer_to_write -> out) >= buffer_to_write -> buffer_size) {

        int result = buffer_to_write -> in - buffer_to_write -> out;
        if(result >= buffer_to_write -> buffer_size) {
            result = result % (buffer_to_write -> buffer_size);
        }

        free(buffer_to_write -> messages[result]);
        buffer_to_write -> messages[result] = create_message();
        buffer_to_write -> in += 1;

        return 1;

    } else {
        for(int i = 0; i < buffer_to_write -> buffer_size; i++) {
            if(buffer_to_write -> messages[i] == NULL) {
                buffer_to_write -> messages[i] = create_message();
                buffer_to_write -> in += 1;
                return 1;
            }
        }
    }
}


int read_message_from_buffer(struct ring_buffer * buffer_to_read) {

    if(buffer_to_read == NULL || (buffer_to_read -> in == buffer_to_read -> out )) {
        printf("BUFFER DIDN'T CREATE OR EMPTY\n");
        return -1;
    }

    for(int i = 0; i < buffer_to_read -> buffer_size; i++) {

        if(buffer_to_read -> messages[i] != NULL) {
            print_message(buffer_to_read -> messages[i]);
            free(buffer_to_read -> messages[i]);
            buffer_to_read -> messages[i] = NULL;
            buffer_to_read -> out += 1;
            break;
        }
    }
} 


unsigned char * create_message() {
    struct timespec temp_time;
    clock_gettime(CLOCK_REALTIME, &temp_time);
    srand(temp_time.tv_nsec);
    int size = rand() % 256; 
    unsigned char * temp_message = malloc(sizeof(unsigned char) * size + sizeof(unsigned char));  
    if(size == 256) {
        temp_message[0] = (char)255;
    } else {
        temp_message[0] = (char)size;
    }
    for(int i = 1; i < (size + 1); i++) {
        temp_message[i] = (char)(48 + rand() % 78);
    }
    return temp_message;
}

void print_message(const unsigned char * message) {
    if(message == NULL) return;
    for(int i = 0; i < (int)message[0] + 1; i++) {
        if(i % 50 == 0) putchar('\n');
        else printf("%c", message[i]);
    }
}

