#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include<stdio.h>
#include<stdlib.h>
// #include<conio.h>

#include <pthread.h>
#include <time.h>
#include <assert.h>


struct node* get_element(struct node *list, int find_index);
void remove_element(int key);
void init_hash_table();
int read (int key);
int read_bucket (int key);


void delete_table();



void test_can_add_multiple_items();
void test_can_get_multiple_item();



struct hashtable *hash_table;


/* Node for storing an item in a Linked List */
struct node 
{
	int key;
	int value;
	struct node *next;
};
 
/* For storing a Linked List at each index of Hash Table  */
struct arrayitem 
{
 
	struct node *head;   
	/* head pointing the first element of Linked List at an index of Hash Table */
 
	struct node *tail;   
	/* tail pointing the last element of Linked List at an index of Hash Table */
 
};

struct hashtable
{
 
	struct arrayitem *bucket_array; 
	/* head pointing the first element of Linked List at an index of Hash Table */
 
	pthread_rwlock_t *lock_table;  
	/* tail pointing the last element of Linked List at an index of Hash Table */
 
};


#endif
