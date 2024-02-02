

#include "hash_table.h"


int size = 0;         /* Determines the no. of elements present in Hash Table */
int max = 10;	      /* Determines the maximum capacity of Hash Table array */

 
/* This function creates an index corresponding to the every given key */
int hashcode(int key)
{
	return (key % max);
}
 

 
void insert(int key, int value)
{
  	// float n = 0.0;     
	/* n => Load Factor, keeps check on whether rehashing is required or not */

    printf("insert start \n");
 
	int index = hashcode(key);
    int ret = 0;


    assert(index <= max);
    pthread_rwlock_wrlock(&hash_table->lock_table[index]);
 
	/* Extracting Linked List at a given index */
	struct node *list = (struct node*) hash_table->bucket_array[index].head;
 
	/* Creating an item to insert in the Hash Table */
	struct node *item = (struct node*) malloc(sizeof(struct node));
	item->key = key;
	item->value = value;
	item->next = NULL;
 
	if (list == NULL) {
		/* Absence of Linked List at a given Index of Hash Table */
 
		printf("Inserting %d(key) and %d(value) \n", key, value);
		hash_table->bucket_array[index].head = item;
		hash_table->bucket_array[index].tail = item;
		// size++;
 
	} else {
		/* A Linked List is present at given index of Hash Table */
		int find_index = find(list, key); 
		if (find_index == -1) {

			/*
			 *Key not found in existing linked list
			 *Adding the key at the end of the linked list
			*/
 
			hash_table->bucket_array[index].tail->next = item;
			hash_table->bucket_array[index].tail = item;
			// size++;
 
		} else {
			/*
			 *Key already present in linked list
			 *Updating the value of already existing key
			*/
			struct node *element = get_element(list, find_index);
			element->value = value;
		}
 
	}

    pthread_rwlock_unlock(&hash_table->lock_table[index]);
 
	// //Calculating Load factor
	// n = (1.0 * size) / max;
	// if (n >= 0.75) 
    //     {
	// 	//rehashing
 
	// 	printf("going to rehash\n");
	// 	rehash();
 
	// }
 
}
 
void rehash()
{
	struct arrayitem *temp = hash_table->bucket_array;     
	/* temp pointing to the current Hash Table array */
 
	int i = 0, n = max;
	size = 0;
	max = 2 * max;
 
	/*
	 *array variable is assigned with newly created Hash Table
	 *with double of previous array size
	*/
	hash_table->bucket_array = (struct arrayitem*) malloc(max * sizeof(struct node));
	init_hash_table(10);
 
	for (i = 0; i < n; i++) 
        {
 
		/* Extracting the Linked List at position i of Hash Table array */
 		struct node* list = (struct node*) temp[i].head;
 
		if (list == NULL) {
			/* if there is no Linked List, then continue */
			continue;
		} else {
			/*
			 *Presence of Linked List at i
			 *Keep moving and accessing the Linked List item until the end.
			 *Get one key and value at a time and add it to new Hash Table array.
			*/
			while (list != NULL) {
				insert(list->key, list->value);
				list = list->next;
			}
		}
	}
	temp = NULL;
}
 
/*
 *This function finds the given key in the Linked List
 *Returns it's index
 *Returns -1 in case key is not present
*/
int find(struct node *list, int key)
{
	int retval = 0;
	struct node *temp = list;
	while (temp != NULL) 
        {
		if (temp->key == key)
                {
			return retval;
		}
  		temp = temp->next;
		retval++;
	}
	return -1;
 
}
 
/* Returns the node (Linked List item) located at given find_index  */
struct node* get_element(struct node *list, int find_index)
{
	int i = 0;
	struct node *temp = list;
	while (i != find_index) 
        {
		temp = temp->next;
		i++;
	}
	return temp;
}
 
/* To remove an element from Hash Table */ 
void remove_element(int key)
{
	int index = hashcode(key);

    pthread_rwlock_wrlock(&hash_table->lock_table[index]);

	struct node *list = (struct node*) hash_table->bucket_array[index].head;
 
	if (list == NULL) {
		printf("This key does not exists\n");

	} else {
		int find_index = find(list, key);
 
		if (find_index == -1) {
			printf("This key does not exists\n");
 
		} else {
 			struct node *temp = list;
			if (temp->key == key) {
  				hash_table->bucket_array[index].head = temp->next;
				printf("This key has been removed\n");
                pthread_rwlock_unlock(&hash_table->lock_table[index]);
				return;
			}
 
			while (temp->next->key != key) {
 				temp = temp->next;
			}
 
  			if (hash_table->bucket_array[index].tail == temp->next) {
				temp->next = NULL;
				hash_table->bucket_array[index].tail = temp;
			} else {
				temp->next = temp->next->next;
			}
			printf("This key has been removed\n");
		}
	}
    
    pthread_rwlock_unlock(&hash_table->lock_table[index]);
}

int read (int key) {
    int index = hashcode(key);
    int res = -1;

    pthread_rwlock_rdlock(&hash_table->lock_table[index]);
	struct node *list = (struct node*) hash_table->bucket_array[index].head;
 
	if (list == NULL) {
		printf("This key does not exists\n");
        goto Unlock;
	} else {
		int find_index = find(list, key);
 
		if (find_index == -1) {
			printf("This key does not exists\n");
            goto Unlock;
		} else {
			struct node *element = get_element(list, find_index);
			res = element->value;
		}
	}

Unlock: 
    pthread_rwlock_unlock(&hash_table->lock_table[index]);
    return res;
}


int read_bucket (int key) {
    int index = hashcode(key);
    int res = -1;


    pthread_rwlock_rdlock(&hash_table->lock_table[index]);
	struct node *list = (struct node*) hash_table->bucket_array[index].head;
 
	if (list == NULL) {
		printf("This key does not exists\n");
        goto Unlock;
	} else {
		int find_index = find(list, key);
 
		if (find_index == -1) {
			printf("This key does not exists\n");
            goto Unlock;
		} else {
			struct node *element = get_element(list, find_index);
			res = element->value;
		}
	}

Unlock: 
    pthread_rwlock_unlock(&hash_table->lock_table[index]);
    return res;
}


 
/* To display the contents of Hash Table */
void display()
{
	int i = 0;
	for (i = 0; i < max; i++) 
        {
		struct node *temp = hash_table->bucket_array[i].head;
		if (temp == NULL) 
                {
			printf("array[%d] has no elements\n", i);
 
		}
                else 
                {
			printf("array[%d] has elements-: ", i);
			while (temp != NULL)
                        {
				printf("key= %d  value= %d\t", temp->key, temp->value);
				temp = temp->next;
			}
			printf("\n");
 
		}
	}
}
 
/* For initializing the Hash Table */
void init_hash_table(ssize_t size) {
	int i = 0;
    int ret = 0;

	assert(size > 0);

	max = size;

    hash_table = (struct hashtable*)malloc(sizeof(struct hashtable));
	hash_table->bucket_array = (struct arrayitem*) malloc(max * sizeof(struct arrayitem));
    hash_table->lock_table = (pthread_rwlock_t*) malloc(max * sizeof(pthread_rwlock_t));

	for (i = 0; i < max; i++)
        {
		hash_table->bucket_array[i].head = NULL;
		hash_table->bucket_array[i].tail = NULL;
        ret = pthread_rwlock_init(&hash_table->lock_table[i], NULL);
        if (ret < 0) {
            printf("pthread_rwlock_init failed {%d}", ret);
            exit(ret);
        }
	}
 
}



/* For initializing the Hash Table */
void delete_table () {
	int index = 0;


	while (index < max) {
		pthread_rwlock_wrlock(&hash_table->lock_table[index]);

		struct node *list = (struct node*) hash_table->bucket_array[index].head;

		if (list == NULL)
			goto Unlock;
		

		struct node *temp = list;
		while (temp != NULL) {
			struct node *cur = temp;
  			temp = temp->next;
			free(cur);
		}

		Unlock:
			index++;
			pthread_rwlock_unlock(&hash_table->lock_table[index]);
	}
	
	// free table
	free(hash_table->bucket_array);
	free(hash_table->lock_table);
	free(hash_table);
}


 
/* Returns size of Hash Table */ 
int size_of_array()
{
	return size;
}



void test_can_add_multiple_items() {


    for (int i = 0; i < 100; i++) {
        insert(rand(), i);
    }
}

void test_can_get_multiple_item() {

    int test_arr[1000] = {};

    for (int i = 0; i < 1000; i++) {
        test_arr[i] = rand();
        insert(test_arr[i], i);
    }


    for (int i = 0; i < 1000; i++) {
        int val = read(test_arr[i]);
        assert(val == i);
    }
}









