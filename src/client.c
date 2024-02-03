

// #include "hash_table.h"
// #include "queue.h"
// #include <sys/types.h>
// #include <unistd.h>
// #include <sys/syscall.h>
// #include <string.h>

#include "ipc_api.h"



void main() 
{
	int choice, key, value, n, c, i, res;
	uint64_t resp_size;
	struct ipc_req req;

	open_queue();
	uint64_t size = req_queue->header->node_data_size;

	assert (size == sizeof(struct ipc_req));

	do {
		printf("Implementation of Hash Table in C chaining with Singly Linked List \n\n");
		printf("MENU-: \n1.Inserting item in the Hash Table"
                              "\n2.Removing item from the Hash Table"
                              "\n3.Read element in bucket" 
                              "\n4.Read bucket"
								"\n5.display the queue"
		       "\n\n Please enter your choice -: ");
 
 		scanf("%d", &choice);
 
		switch(choice) {
 
		case 1:
		    printf("Inserting element in Hash Table\n");
		    printf("Enter key and value-:\t");
		    scanf("%d %d", &key, &value);

			req.req_type = insert_req;
			struct insert_req *insert = (struct insert_req *)req.buf;
			insert->key = key;
			insert->value = value;
			LFQueue_push(req_queue, &req, size, NULL);
			break;
		case 2:
		    printf("Deleting in Hash Table \nEnter the key to delete-:");
		    scanf("%d", &key);
		    //remove_element(key);
			req.req_type = delete_req;
			struct delete_req *delete = (struct delete_req *)req.buf;
			delete->key = key;
			LFQueue_push(req_queue, &req, size, NULL);
		    break;
		case 3:
			printf("Read value in the queue \n Enter the key-:");
			scanf("%d", &key);

	
			req.req_type = read_element_req;
			struct read_element_req *read_element = (struct read_element_req *)req.buf;
			read_element->key = key;

			LFQueue_push(req_queue, &req, size, NULL);


			LFQueue_pop(rsp_queue, &req, &resp_size, NULL);

			struct read_element_rsp *rsp = (struct read_element_rsp *)req.buf;

			if (rsp->is_exist) {
				printf("key %d  value %d \n", rsp->key, rsp->value);
			} else {
				printf("key not exist\n");
			}
		    break;
		case 4:
			printf("Read bucket \n Enter the key-:");
			scanf("%d", &key);

			req.req_type = read_bucket_req;
			struct read_bucket_req *read_elm = (struct read_bucket_req *)req.buf;
			read_elm->key = key;

			LFQueue_push(req_queue, &req, size, NULL);


			LFQueue_pop(rsp_queue, &req, &resp_size, NULL);

			struct read_bucket_rsp *b_rsp = (struct read_bucket_rsp *)req.buf;

			if (b_rsp->is_exist) {
				printf("Key value in bucket \n");
				for (int i = 0; i < b_rsp->depth; i++) {
					printf("key %d  value %d \n", b_rsp->value[i][0],  b_rsp->value[i][1]);
				}
	
			} else {
				printf("key not exist\n");
			}
		    break;
		case 5:
			 LFQueue_print(req_queue);
 
		default:
		       printf("Wrong Input\n");
		}
 
		printf("\nDo you want to continue-:(press 1 for yes)\t");
		scanf("%d", &c);
 
	}while(c == 1);
 
	getchar();

}