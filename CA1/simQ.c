#include <stdio.h>
#include <stdlib.h>
#include "randnorm.h"
#include "customer.h"
#include "queue.h"
#include "parser.h"

int main(int argc, char* argv[]) {

    if (argc != 4) {
        fprintf(stderr, "\nIncorrect usage.\nSyntax: simQ [fileIn] [numSims] [fileOut]\n\n");
        exit(EXIT_FAILURE);
    }

    FILE *inputFile;
    if (!(inputFile = fopen(argv[1], "r"))) {
        perror("Input file could not be opened");
        exit(EXIT_FAILURE);
    }

    const int MAX_QUEUE_LENGTH = read_config_value(inputFile, "maxQueueLength");
    printf("Maximum queue length: %i\n", MAX_QUEUE_LENGTH);

    const int NUM_SERVICE_POINTS = read_config_value(inputFile, "numServicePoints");
    printf("Number of service points: %i\n", NUM_SERVICE_POINTS);

    const int CLOSING_TIME = read_config_value(inputFile, "closingTime");
    printf("Closing time: %i\n", CLOSING_TIME);

    fclose(inputFile);

    srand(time(NULL));

    int NUM_CUSTOMERS = 10;
    Queue* serviceQueue = new_queue(MAX_QUEUE_LENGTH);

    unsigned int i;
    for (i = 0; i < NUM_CUSTOMERS;) {
        Customer* C = create_customer(i, 10, 5, 30, 10);
        printf("%s", customer_to_string(C));
        enqueue(serviceQueue, C);
        ++i;
    }

    Customer* firstInLine = dequeue(serviceQueue);
    printf("%s", customer_to_string(firstInLine));

    firstInLine = dequeue(serviceQueue);
    printf("%s", customer_to_string(firstInLine));

    free(firstInLine);

    free(serviceQueue);
    return 0;
}