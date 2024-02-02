
#include "hash_table.h"
#include "queue.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

#define SHARED_KEY  -1

LFQueue *queue;
bool overite = false;
bool pause_before_print = false;
char filename[128] = "lfq.dat";



void create_mq()
{
        if (LFQueue_create(SHARED_KEY, 128, 128, overite) != 0) {
                perror("[FAILURE] Error occured in LFQueue_create");
                exit(-1);
        }
        printf("[SUCESS] Create LFQueue\n");
}

void destroy_mq()
{
        if (LFQueue_destroy(SHARED_KEY) != 0) {
                perror("[FAILURE] Error occured in LFQueue_destroy");
                exit(-1);
        }
}

void *produce_task(void *arg)
{
        uint64_t size = queue->header->node_data_size;
        void *buf = malloc(size);
        for (;;)
                LFQueue_push(queue, buf, size, NULL);
}

void *consume_task(void *arg)
{
        uint64_t size = queue->header->node_data_size;
        void *buf = malloc(size);
        for (;;)
                LFQueue_pop(queue, buf, &size, NULL);
}

void produce()
{
        int i;

        queue = LFQueue_open(SHARED_KEY);
        if (!queue) {
                printf("[FAILURE] Error occured in LFQueue_open\n");
                exit(-1);
        }

        for (i = 0; i < 10; ++i) {
                pthread_t th;
                pthread_create(&th, NULL, produce_task, NULL);
        }

        produce_task(NULL);
}

void consume()
{
        int i;

        queue = LFQueue_open(SHARED_KEY);
        if (!queue) {
                printf("[FAILURE] Error occured in LFQueue_open\n");
                exit(-1);
        }

        for (i = 0; i < 10; ++i) {
                pthread_t th;
                pthread_create(&th, NULL, consume_task, NULL);
        }

        consume_task(NULL);
}





void* main_thread(void *arg) {
    test_can_get_multiple_item();
    pid_t pid = syscall(__NR_gettid);

    printf("11111 pid %d\n", pid);
    
    char buf[64];
    const char *data = "hello, world!";
    LFQueue_push(queue, data, strlen(data) + 1, NULL);
    // LFQueue_pop(queue, buf, strlen(data) + 1, NULL);
    // printf("%s, pid %d\n", buf, pid);
    return NULL;
}

void main() 
{
	int choice, key, value, n, c, i;

    pthread_t threads[10];

    init_hash_table();
    srand(time(NULL));


    create_mq();

    produce();
    consume();

    destroy_mq();


    // LFQueue_create(SHARED_KEY, 128, 128, true); 

    // for (i = 0; i < 100; i++)
    //     pthread_create(&threads[i], NULL, main_thread, NULL);
    // for (i = 0; i < 100; i++)
    //     pthread_join(threads[i], NULL);


    // LFQueue_close(queue);
    // LFQueue_destroy(SHARED_KEY);


    delete_table();
 
}