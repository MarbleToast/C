#ifndef CUSTOMER_H
#define CUSTOMER_H

typedef struct customer {
    unsigned int id;
    unsigned int serviceTime;
    unsigned short waitingTolerance;
    unsigned int waitingTime;
} Customer;

char* customerToString(Customer* c);

Customer* createCustomer(
    unsigned int id,
    double st_mean,
    double st_sd,
    double wt_mean,
    double wt_sd
);

#endif
