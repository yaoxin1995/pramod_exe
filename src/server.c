
#include "hash_table.h"


void* main_thread(void *arg) {
    test_can_get_multiple_item();
    return NULL;
}

void main() 
{
	int choice, key, value, n, c, i;

    pthread_t threads[10];

    init_hash_table();
    srand(time(NULL));  

    for (i = 0; i < 10; ++i)
        pthread_create(&threads[i], NULL, main_thread, NULL);
    for (i = 0; i < 10; ++i)
        pthread_join(threads[i], NULL);


    delete_table();
 
}