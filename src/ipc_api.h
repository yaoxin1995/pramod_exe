#ifndef _IPC_API_H_
#define _IPC_API_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "hash_table.h"
#include "queue.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>


#define SHARED_KEY_RQ  -2
#define SHARED_KEY_RSP  -3

LFQueue * req_queue;
LFQueue * rsp_queue;



typedef enum {
    insert_req, 
    read_bucket_req,
    read_element_req, 
    delete_req,
    read_bucket_rsp,
    read_element_rsp,
    delete_rsp,

} Mode;


struct ipc_req {
	Mode req_type;
	char buf[256];
};

struct insert_req {
	int key;
	int value;
};

struct read_element_req {
	int key;
};

struct read_element_rsp {
    bool is_exist;
	int key;
    int value;
};

struct read_bucket_req {
	int key;
};


struct read_bucket_rsp {
    bool is_exist;
	int key;
    int depth;
    int value[40][2];
};


struct delete_req {
	int key;
};


void create_mq();
void destroy_mq();
void open_queue();
void* server_consume_req (void *arg);



#endif
