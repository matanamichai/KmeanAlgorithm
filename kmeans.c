#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define  _POSIX_C_SOURCE 200809L
#define MAX_ITER 1000

typedef struct cord
{
    double value;
    struct cord *next;
} cord ;

typedef struct vector
{
    cord *cords;
    struct vector *next;
} vector ;

vector *fillDataPoint();
int validateIter(char *iter);
int isNaturalNumber(char *c);
int isdigit(int);
int validateK(char *k, vector *pointsVector);
int countPointsInVector(vector *pointsVector);
void printVector(vector*);
void printCord(cord*);


vector *fillDataPoint(){
    vector *head_vec, *curr_vec, *next_vec;
    cord *head_cord, *curr_cord, *next_cord;
    double n;
    char c;

    head_cord = malloc(sizeof(cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;
    while (scanf("%lf%c", &n, &c) == 2)
    {

        if (c == '\n')
        {
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }
    return head_vec;
}

void printVector(vector *v) {
    while (v->next != NULL) {
        printCord(v->cords);
        v = v->next;
    }
    printf("\n");
}

void printCord(cord *c) {
    do {
        printf("%.4f,", c->value);
        c = c->next;
    }
    while (c->next != NULL);
    printf("%.4f\n", c->value);
}

int validateIter(char *iter) {
    int intIter;
    
    if (isNaturalNumber(iter) != 0) {
        printf("Invalid maximum iteration!\n");
        return 1;    
    }

    intIter = atoi(iter);
    if (intIter >= MAX_ITER || intIter <= 1) {        
        printf("Invalid maximum iteration!\n");
        return 2;
    }

    return 0;
}

int validateK(char *k, vector *pointsVector) {
    int points;
    int intK;
    
    if (isNaturalNumber(k) != 0) {
        printf("Invalid number of clusters!\n");
        return 1;
    }

    points = countPointsInVector(pointsVector);
    intK = atoi(k);
    if (intK >= points || intK <= 1) {
        printf("Invalid number of clusters!\n");
        return 2;
    }

    return 0;
}

int countPointsInVector(vector *v) {
    int counter = 0;
    while (v->next != NULL) {
        counter++;
        v = v->next;
    }

    return counter;
}

// returns 0 if it is a natural number, and 1 otherwise
int isNaturalNumber(char *c) {
    while (*c != '\0') {
        if (isdigit(*c) == 0) {
            return 1;    
        }

        c++;
    }

    return 0;
}

int main(int argc, char *argv[]){
    vector *pointsVector;
    vector *KCenterVector;
    int maxOfIter;
    int valid;
    int k;

    if (argc > 2){
        valid = validateIter(argv[2]);
        if (valid != 0) {
            return 1;
        }

        maxOfIter = atoi(argv[2]);
    }
    else{
        maxOfIter = 200;
    }
    
    pointsVector = fillDataPoint();
    valid = validateK(argv[1], pointsVector);
    if (valid != 0) {
        return 1;
    }

    k = atoi(argv[1]);

    printVector(pointsVector);   
    return 0;
}