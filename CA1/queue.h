#ifndef QUEUE_H
#define QUEUE_H

#include "customer.h"

typedef struct {
    unsigned int head;
    unsigned int tail;
    unsigned int size;
    unsigned int max_size;

    Customer** queue;
} Queue;

Queue* new_queue(unsigned int max_size);

int enqueue(Queue* q, Customer* c);
Customer* dequeue(Queue* q);

int is_full(Queue* q);
int is_empty(Queue* q);

#endif