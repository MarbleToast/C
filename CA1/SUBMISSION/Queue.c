#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

/*
    Creates an empty Queue with a defined max length.
    @param max: Maximum length of the queue.
    @returns Pointer to a Queue instance, where q->maxSize = [max]
*/
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

/*
    Creates a new QueueNode, then appends it to Queue q.
    @param q: Pointer to a Queue instance
    @param c: Pointer to a Customer instance
    @returns 0 if queue full, 1 if added successfully.
*/
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

/*
    Gets the Customer in the head node and frees the head node.
    @param q: Pointer to a Queue instance
    @returns Pointer to a Customer instance. 
*/
Customer* dequeue(Queue* q) {

    Customer* c;
    QueueNode* prevHead;

    if (q->head == NULL || q->size == 0) return NULL;

    prevHead = q->head;
    c = prevHead->customer;

    q->head = q->head->next;
    free(prevHead);

    if (q->head == NULL) {
        q->tail = NULL;
        q->size = 0;
        return c;
    }

    q->head->previous = NULL;
    q->size -= 1;

    return c;
}

/*
    Gets the Customer in the node at the given key and frees the node.
    @param q: Pointer to a Queue instance
    @param key: the integer key for the node
    @returns Pointer to a Customer instance. 
*/
Customer* splice(Queue* q, unsigned int key) {
    Customer* customer;
    QueueNode* node = q->head;

    while (node != NULL) {

        if (node->key == key) {

            if (q->size > 1) {
                /* Account for head or tail node placement */
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

/*
    Frees all remaining nodes in a Queue, effectively emptying it.
    @param q: Pointer to a Queue instance.
*/
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
