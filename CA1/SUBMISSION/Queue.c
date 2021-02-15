#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue* newQueue(unsigned int max) {
    Queue* q = NULL;

    if (!(q = malloc(sizeof *q))) {
        perror("[ERROR] Queue could not be made");
        exit(EXIT_FAILURE);
    };

    q->head = NULL;
    q->tail = NULL;

    q->size = 0;
    q->maxSize = max;

    return q;
}

int enqueue(Queue* q, Customer* c) {

    QueueNode* node;

    if (q->size == q->maxSize) return 0;

    if (!(node = malloc(sizeof * node))) {
        perror("[ERROR] Customer could not be added");
        exit(EXIT_FAILURE);
    };

    node->customer = c;
    node->key = c->id;
    node->next = NULL;
    node->previous = NULL;

    if (q->head == NULL) {
        q->head = node;
        q->tail = node;
        q->size += 1;
        return 1;
    }
    
    node->previous = q->tail;

    q->tail->next = node;
    q->tail = node;
    q->size += 1;

    return 1;
}

Customer* dequeue(Queue* q) {

    Customer* c;

    if (q->head == NULL || q->size == 0) return NULL;

    c = q->head->customer;
    free(&q->head);

    q->head = q->head->next;

    if (q->head == NULL) {
        q->tail = NULL;
        q->size = 0;
        return c;
    }

    q->head->previous = NULL;
    q->size -= 1;

    return c;
}

Customer* splice(Queue* q, unsigned int key) {
    Customer* customer;
    QueueNode* node = q->head;

    while (node != NULL) {

        if (node->key == key) {

            if (q->size > 1) {
                if (node->next != NULL) {
                    node->next->previous = node->previous;
                }
                else {
                    q->tail = node->previous;
                }

                if (node->previous != NULL) {
                    node->previous->next = node->next;
                }
                else {
                    q->head = node->next;
                }
            } else {
                q->head = NULL;
                q->tail = NULL;
            }
            customer = node->customer;
            free(node);
            q->size--;
            return customer;
        }
        node = node->next;
    }
    return NULL;
}

void freeQueue(Queue* q) {

    QueueNode* temp;

    while (q->head != NULL) {
        temp = q->head;
        q->head = q->head->next;
        free(temp->customer);
        free(temp);
    }

    q->tail = NULL;
}
