#ifndef CUSTOMER_H
#define CUSTOMER_H

typedef enum {
    WAITING,
    TIMED_OUT,
    FUFILLED,
    UNFUFILLED
} CustomerState;

typedef struct {
    unsigned int id;
    unsigned int serviceTime;
    unsigned short waitingTolerance;
    CustomerState state;
} Customer;

char* customer_to_string(Customer* c);

Customer* create_customer(
    unsigned int id,
    double st_mean,
    double st_sd,
    double wt_mean,
    double wt_sd
);

#endif