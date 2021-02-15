/* simQ.c
    Program entry point.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "RandNorm.h"
#include "Customer.h"
#include "Queue.h"
#include "CustomerParser.h"

int main(int argc, char* argv[]) {

    FILE* inputFile;
    FILE* outputFile;

    char* stop;

    int NUM_SIMULATIONS;
    unsigned int MAX_QUEUE_LENGTH;
    unsigned int NUM_SERVICE_POINTS;
    unsigned int CLOSING_TIME;
    unsigned int NUM_CUSTOMERS_MEAN;
    int NUM_CUSTOMERS_SD;
    unsigned int CUSTOMER_SERVICE_TIME_MEAN;
    int CUSTOMER_SERVICE_TIME_SD;
    unsigned int CUSTOMER_TOLERANCE_MEAN;
    int CUSTOMER_TOLERANCE_SD;
    unsigned int customerQuantity;

    unsigned int i;
    unsigned int timeStep;
    unsigned int customerID;
    unsigned int serviceSpace;

    unsigned int totalUnfufilled = 0;
    unsigned int totalFufilled = 0;
    unsigned int totalTimedOut = 0;
    unsigned int totalStepsAfterClosingTime = 0;
    unsigned int totalFufilledWaitingTime = 0;
    unsigned int totalWaitingTimeFufilled = 0;

    unsigned int* customersAdditionSteps;

    Queue* serviceQueue;
    Queue* servicePoints;
    Queue* timedOutCustomers;
    Queue* unfufilledCustomers;
    Queue* fufilledCustomers;

    QueueNode* prevWorking;
    QueueNode* prevWaiting;
    QueueNode* waiting;
    QueueNode* working;

    unsigned int simNum;

    Customer* C;

    unsigned int totalCustomers;

    /* Command line arguments must be equal to 4: simQ, input file, sim count, output file */
    if (argc != 4) {
        fprintf(stderr, "\nIncorrect usage.\nSyntax: simQ [fileIn] [numSims] [fileOut]\n\n");
        exit(EXIT_FAILURE);
    }

    /* Open input file and output file in reading and writing mode respectively*/
   
    if (!(inputFile = fopen(argv[1], "r"))) {
        perror("Input file could not be opened");
        exit(EXIT_FAILURE);
    }

    
    if (!(outputFile = fopen(argv[3], "w"))) {
        perror("Output file could not be opened");
        exit(EXIT_FAILURE);
    }

    /* === BEGIN PARAMETER READING === */

    /* NUM_SIMULATIONS: Number of simulations to run. Must be >= 1. */
    /* convert numSims argument to a number to use */
    NUM_SIMULATIONS = strtol(argv[2], &stop, 10);
    if (NUM_SIMULATIONS < 1) {
        fprintf(stderr, "The simulation count must be greater than or equal to 1.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(outputFile, "Sim count: %s\n", argv[2]);

    /* MAX_QUEUE_LENGTH: Maximum number of customers able to sit the waiting queue. Non-negative. */
    MAX_QUEUE_LENGTH = readConfigValue(inputFile, "maxQueueLength", 0);
    fprintf(outputFile, "Maximum queue length: %i\n", MAX_QUEUE_LENGTH);

    /* NUM_SERVICE_POINTS: Number of customers able to be served simultaneously. Non-negative. */
    NUM_SERVICE_POINTS = readConfigValue(inputFile, "numServicePoints", 0);
    fprintf(outputFile, "Number of service points: %i\n", NUM_SERVICE_POINTS);

    /* CLOSING_TIME: Number of time steps where customers may be added to the waiting queue. Must be >= 1. */
    CLOSING_TIME = readConfigValue(inputFile, "closingTime", 0);
    fprintf(outputFile, "Closing time: %i\n", CLOSING_TIME);

    /* NUM_CUSTOMERS_MEAN: Mean number of customers per simulation. Must be >= 1. */
    NUM_CUSTOMERS_MEAN = readConfigValue(inputFile, "numCustomersMean", 0);
    fprintf(outputFile, "Number of customers mean: %i\n", NUM_CUSTOMERS_MEAN);

    /* NUM_CUSTOMERS_SD: Variance in number of customers per simulation. May be negative, but not <= -NUM_CUSTOMERS_MEAN. */
    NUM_CUSTOMERS_SD = readConfigValue(inputFile, "numCustomersSD", 1);
    fprintf(outputFile, "Number of customers standard deviation: %i\n", NUM_CUSTOMERS_SD);

    /* CUSTOMER_SERVICE_TIME_MEAN: Mean service time for customers per simulation. Must be >= 1. */
    CUSTOMER_SERVICE_TIME_MEAN = readConfigValue(inputFile, "customerServiceTimeMean", 0);
    fprintf(outputFile, "Customer service time mean: %i\n", CUSTOMER_SERVICE_TIME_MEAN);

    /* CUSTOMER_SERVICE_TIME_SD: Variance in service time for customers per simulation. May be negative, but not <= -CUSTOMER_SERVICE_TIME_MEAN. */
    CUSTOMER_SERVICE_TIME_SD = readConfigValue(inputFile, "customerServiceTimeSD", 1);
    fprintf(outputFile, "Customer service time standard deviation: %i\n", CUSTOMER_SERVICE_TIME_SD);

    /* CUSTOMER_TOLERANCE_MEAN: Mean waiting tolerance for customers per simulation. Must be >= 1. */
    CUSTOMER_TOLERANCE_MEAN = readConfigValue(inputFile, "customerToleranceMean", 0);
    fprintf(outputFile, "Customer tolerance mean: %i\n", CUSTOMER_TOLERANCE_MEAN);

    /* CUSTOMER_TOLERANCE_SD: Variance waiting tolerance for customers per simulation. May be negative, but not <= -CUSTOMER_TOLERANCE_MEAN. */
    CUSTOMER_TOLERANCE_SD = readConfigValue(inputFile, "customerToleranceSD", 1);
    fprintf(outputFile, "Customer tolerance standard deviation: %i\n", CUSTOMER_TOLERANCE_SD);

    fclose(inputFile);

    /* Seed RNG */
    srand((unsigned int)time(NULL));

    serviceQueue = newQueue(MAX_QUEUE_LENGTH);
    servicePoints = newQueue(NUM_SERVICE_POINTS);

    /* BEGIN SIMULATIONS LOOP */
    
    for (simNum = 0; simNum < NUM_SIMULATIONS;) {

        customerQuantity = 0;
        while (customerQuantity == 0) {
            customerQuantity = randNorm(NUM_CUSTOMERS_MEAN, NUM_CUSTOMERS_SD);
            if (customerQuantity == 0) {
                printf("Got zero customers, retrying...");
            }
        }

        customersAdditionSteps = randNumsToSum(customerQuantity, CLOSING_TIME);

        timedOutCustomers = newQueue(customerQuantity);
        unfufilledCustomers = newQueue(customerQuantity);
        fufilledCustomers = newQueue(customerQuantity);

        timeStep = 0;
        customerID = 0;

        while (serviceQueue->size > 0 || servicePoints->size > 0 || timeStep == 0 || timeStep < CLOSING_TIME) {
            if (NUM_SIMULATIONS == 1) {
                fprintf(outputFile,
                    "Time step %u: %u being served, %u waiting, %u fufilled, %u timed out, %u unfufilled\n",
                    timeStep,
                    servicePoints->size,
                    serviceQueue->size,
                    fufilledCustomers->size,
                    timedOutCustomers->size,
                    unfufilledCustomers->size
                );
            }

            working = servicePoints->head;
            while (working != NULL) {
                working->customer->serviceTime--;
		        prevWorking = working;
		        working = working->next;

                if (prevWorking->customer->serviceTime < 1) {
                    totalFufilledWaitingTime += prevWorking->customer->waitingTime;
                    enqueue(fufilledCustomers, splice(servicePoints, prevWorking->key));
                }
            }

            serviceSpace = servicePoints->size;
            for (i = 0; i < (servicePoints->maxSize - serviceSpace);) {
                if (serviceQueue->head != NULL) {
                    enqueue(servicePoints, splice(serviceQueue, serviceQueue->head->key));
                }
                ++i;
            }

            waiting = serviceQueue->head;
            while (waiting != NULL) {
                waiting->customer->waitingTime++;
                waiting->customer->waitingTolerance--;
		        prevWaiting = waiting;
		        waiting = waiting->next;

                if (prevWaiting->customer->waitingTolerance < 1) {
                    enqueue(timedOutCustomers, splice(serviceQueue, prevWaiting->key));
                }
            }

            if (timeStep < CLOSING_TIME) {
                for (i = 0; i < customersAdditionSteps[timeStep];) {
                    C = createCustomer(
                        customerID++,
                        CUSTOMER_SERVICE_TIME_MEAN,
                        CUSTOMER_SERVICE_TIME_SD,
                        CUSTOMER_TOLERANCE_MEAN,
                        CUSTOMER_TOLERANCE_SD
                    );
                    if (!enqueue(serviceQueue, C)) {
                        if (!enqueue(unfufilledCustomers, C)) {
			                fprintf(stderr, "[ERROR] : Customer could not be added to unfufilled queue.");
			                exit(EXIT_FAILURE);
		                }
                    }
                    ++i;
                }
            }

            timeStep++;
        }

        totalFufilled += fufilledCustomers->size;
        totalUnfufilled += unfufilledCustomers->size;
        totalTimedOut += timedOutCustomers->size;
        totalStepsAfterClosingTime += timeStep - CLOSING_TIME;

        freeQueue(unfufilledCustomers);
        freeQueue(timedOutCustomers);
        freeQueue(fufilledCustomers);

        free(customersAdditionSteps);
        free(unfufilledCustomers);
        free(timedOutCustomers);
        free(fufilledCustomers);
        free(working);
        free(waiting);
        
        ++simNum;
    }

    /* END SIMULATIONS LOOP */

    totalCustomers = totalUnfufilled + totalFufilled + totalTimedOut;

    if (NUM_SIMULATIONS > 1) {
        fprintf(outputFile, "\nNo. of total customers over %u simulations: %u\n", NUM_SIMULATIONS, totalCustomers);
        fprintf(outputFile, "Average no. of unfufilled customers: %.2f\n", (float) totalUnfufilled / NUM_SIMULATIONS);
        fprintf(outputFile, "Average no. of timed-out customers: %.2f\n", (float)totalTimedOut / NUM_SIMULATIONS);
        fprintf(outputFile, "Average no. of fufilled customers: %.2f\n", (float)totalFufilled / NUM_SIMULATIONS);
        fprintf(outputFile, "Average no. of time steps after closing: %.2f\n", (float)totalStepsAfterClosingTime / NUM_SIMULATIONS);
    }
    else {
        fprintf(outputFile, "No. of time steps after closing: %u\n", totalStepsAfterClosingTime);
    }

    fprintf(outputFile, "Average no. of time steps fufilled customers waited for: %.2f\n", (float)totalFufilledWaitingTime / totalFufilled);

    fclose(outputFile);

    free(serviceQueue);
    free(servicePoints);
    
    printf("\nSimulation run %i times, results output to %s.\n", NUM_SIMULATIONS, argv[3]);

    return 0;
}
