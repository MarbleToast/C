#ifndef QUEUE_H
#define QUEUE_H

#include "Customer.h"

typedef struct queueNode {
    struct queueNode* next;
    struct queueNode* previous;

    Customer* customer;
    unsigned int key;
} QueueNode;

typedef struct queue {
    QueueNode* head;
    QueueNode* tail;
    unsigned int size;
    unsigned int maxSize;
} Queue;

Queue* newQueue(unsigned int max);

int enqueue(Queue* q, Customer* c);
Customer* dequeue(Queue* q);
void freeQueue(Queue* q);
Customer* splice(Queue* q, unsigned int key);

#endif
