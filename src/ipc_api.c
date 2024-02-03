#include "ipc_api.h"


bool overite = false;
void create_mq() {
        if (LFQueue_create(SHARED_KEY_RQ, sizeof(struct ipc_req), 128, overite) != 0) {
                perror("[FAILURE] Error occured in LFQueue_create");
                exit(-1);
        }

        if (LFQueue_create(SHARED_KEY_RSP, sizeof(struct ipc_req), 128, overite) != 0) {
                perror("[FAILURE] Error occured in LFQueue_create");
                exit(-1);
        }

        printf("[SUCESS] Create LFQueue\n");
}

void destroy_mq()
{
        if (LFQueue_destroy(SHARED_KEY_RQ) != 0) {
                perror("[FAILURE] Error occured in LFQueue_destroy");
                return;
        }

        if (LFQueue_destroy(SHARED_KEY_RSP) != 0) {
                perror("[FAILURE] Error occured in LFQueue_destroy");
                return;
        }
}


void open_queue() {
        req_queue = LFQueue_open(SHARED_KEY_RQ);
        if (!req_queue) {
                printf("[FAILURE] Error occured in LFQueue_open\n");
                exit(-1);
        }

        rsp_queue = LFQueue_open(SHARED_KEY_RSP);
        if (!req_queue) {
                printf("[FAILURE] Error occured in LFQueue_open\n");
                exit(-1);
        }
}


void* server_consume_req (void *arg) {
        uint64_t size = req_queue->header->node_data_size;
        struct hash_table* table = (struct hash_table*)arg;
        struct insert_req *insert_request;
        struct read_bucket_req *read_buck;
        struct read_element_req *read_ele_req;
        struct delete_req *rm_req;
        struct ipc_req rsp;

        for (;;) {
            struct ipc_req *req = (struct ipc_req*)malloc(size);
            LFQueue_pop(req_queue, req, &size, NULL);
            printf("server get request %d\n", req->req_type);
            switch (req->req_type)
            {
            case insert_req/* constant-expression */:

                insert_request = (struct insert_req*)req->buf;
                printf("insertion request key %d value %d\n", insert_request->key, insert_request->value);
                insert(insert_request->key, insert_request->value);
                break;
            case read_bucket_req/* constant-expression */:
                /* code */
                read_buck= (struct read_bucket_req*)req->buf;
                int depth = 0;
                printf("read_bucket_req request key %d\n", read_buck->key);
                int (*bucket)[2] = read_bucket(read_buck->key, &depth);
                printf("read_bucket_req request key %d\n", read_buck->key);

                struct read_bucket_rsp *read_bucket_rsp = (struct read_bucket_rsp*)rsp.buf;
                read_bucket_rsp->is_exist = depth == -1? false: true;
                read_bucket_rsp->depth = depth;

                if (depth > 0) {
                        for (int i = 0; i < read_bucket_rsp->depth; i++) {
                                read_bucket_rsp->value[i][0] = bucket[i][0];
                                read_bucket_rsp->value[i][1] = bucket[i][1];
                        }
                } else {
                    printf("read_bucket_req key does not exit\n");
                }

                LFQueue_push(rsp_queue, &rsp, size, NULL);
                free(bucket);
                break;

            case read_element_req:
                read_ele_req = (struct read_element_req*)req->buf;
                int element = read_element_in_bucket (read_ele_req->key);
                bool is_exist = element != -1111111 ? true: false;
                printf("read_element_req key %d is_exist %d value %d\n ", read_ele_req->key, is_exist, element);
                rsp.req_type = read_element_rsp;

                struct read_element_rsp* read_element_rsp = (struct read_element_rsp*)rsp.buf;
                read_element_rsp->is_exist = is_exist;
                read_element_rsp->key = read_ele_req->key;
                read_element_rsp->value = element;
                LFQueue_push(rsp_queue, &rsp, size, NULL);
                break;
            case delete_req:
                rm_req = (struct delete_req*)req->buf;
                printf("delete_req key %d\n", rm_req->key);
                remove_element(rm_req->key);
                break;
            
            default:
                break;
            }

            free (req);
        }

}

