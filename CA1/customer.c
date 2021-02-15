#include "customer.h"
#include <stdio.h>
#include <stdlib.h>

char* customer_to_string(Customer* c) {

    char* buff;

    int buff_size = snprintf(
        NULL,
        0,
        "C{id: %d, service_time: %d, tolerance: %d}\n", 
        c->id, 
        c->serviceTime, 
        c->waitingTolerance
    );

    if (!(buff = (char*)malloc(buff_size + 1))) {
        perror("[ERROR] Customer could not be written to string");
        exit(EXIT_FAILURE);
    }

    snprintf(
        buff,
        buff_size + 1, 
        "C{id: %d, service_time: %d, tolerance: %d}\n", 
        c->id, 
        c->serviceTime, 
        c->waitingTolerance
    );

    return buff;
}

Customer* create_customer(
    unsigned int id,
    double st_mean,
    double st_sd,
    double wt_mean,
    double wt_sd
) {
    Customer* C;
    if((C = (Customer*)malloc(sizeof(Customer))) == NULL) {
        perror("[ERROR]");
        exit(EXIT_FAILURE);
    };

    C->id = id;
    C->serviceTime = randnorm(st_mean, st_sd);
    C->waitingTolerance = randnorm(wt_mean, wt_sd);

    return C;
}