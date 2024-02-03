
// #include "hash_table.h"
// #include "queue.h"
// #include <sys/types.h>
// #include <unistd.h>
// #include <sys/syscall.h>
// #include <string.h>

#include "ipc_api.h"

void main() 
{
	int choice, key, value, n, c, i;

    pthread_t threads[10];
    srand(time(NULL));
    init_hash_table(100);
    destroy_mq();
    create_mq();
    open_queue();

    for (i = 0; i < 1; i++)
        pthread_create(&threads[i], NULL, server_consume_req, hash_table);
    for (i = 0; i < 1; i++)
        pthread_join(threads[i], NULL);

    destroy_mq();
    delete_table();
 
}