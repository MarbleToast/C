#include <stdio.h>
#include <stdlib.h>

#include "Customer.h"
#include "RandNorm.h"

char* customerToString(Customer* c) {

    char* buff;

    int buff_size = snprintf(
        NULL,
        0,
        "C{id: %d, serviceTime: %d, tolerance: %d, waitingTime: %d}\n",
        c->id,
        c->serviceTime,
        c->waitingTolerance,
        c->waitingTime
    );

    if (!(buff = malloc(buff_size + 1))) {
        perror("[ERROR] Customer could not be written to string");
        exit(EXIT_FAILURE);
    }

    snprintf(
        buff,
        buff_size + 1,
        "C{id: %d, serviceTime: %d, tolerance: %d, waitingTime: %d}\n",
        c->id,
        c->serviceTime,
        c->waitingTolerance,
        c->waitingTime
    );

    return buff;
}

Customer* createCustomer(
    unsigned int id,
    double st_mean,
    double st_sd,
    double wt_mean,
    double wt_sd
) {
    Customer* C;
    if (!(C = malloc(sizeof(*C)))) {
        perror("[ERROR]");
        exit(EXIT_FAILURE);
    };

    C->id = id;
    C->serviceTime = randNorm(st_mean, st_sd);
    C->waitingTolerance = randNorm(wt_mean, wt_sd);
    C->waitingTime = 0;

    return C;
}
