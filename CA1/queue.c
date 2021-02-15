#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue* new_queue(unsigned int max) {
    Queue* q = NULL;

    if (!(q = (Queue*)malloc(sizeof(Queue)))) {
        perror("[ERROR] Queue could not be made");
        exit(EXIT_FAILURE);
    };

    q->head = 0;
    q->max_size = max;
    q->tail = q->max_size - 1;

    q->size = 0;

    if (!(q->queue = (Customer**)malloc(sizeof(Customer*) * q->max_size))) {
        perror("[ERROR] Queue could not be made");
        exit(EXIT_FAILURE);
    };

    return q;
};

int is_full(Queue* q) {
    return (q->size == q->max_size);
}

int is_empty(Queue* q) {
    return (q->size == 0);
}

int enqueue(Queue* q, Customer* c) {
    if (is_full(q)) return 0;

    q->tail += 1;
    q->tail %= q->max_size;

    q->queue[q->tail] = c;
    q->size += 1;

    return 1;
};

Customer* dequeue(Queue* q) {
    if (is_empty(q)) return;

    Customer* c = q->queue[q->head];

    q->head += 1;
    q->head %= q->max_size;

    q->size -= 1;

    return c;
};