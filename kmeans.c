#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define  _POSIX_C_SOURCE 200809L
#define MAX_ITER 1000
#define EPSILON 0.0001

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

// Structs
int countPointsInVector(vector *pointsVector);
void printVector(vector*);
void printCord(cord*);

// Algorithem 
vector *fillDataPoint();
vector **initializeKCenter(int k,vector *pointsVector);

// Validations
int validateIter(char *iter);
int isNaturalNumber(char *c);
int validateK(char *k, vector *pointsVector);

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

vector **initializeKCenter(int k, vector *points_vector) {
    vector *points_vector_vector;
    cord **clusters;
    cord *head_cord, *curr_cord, *next_cord, *points_vector_cord;
    int i = 0;

    head_cord = malloc(sizeof(cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    clusters = malloc(sizeof(k) * sizeof(cord));
    clusters[i] = head_cord;

    points_vector_vector = points_vector;
    points_vector_cord = points_vector_vector->cords;

    while (k > 0)
    {
        while (points_vector_cord->next != NULL)
        {
            curr_cord->value = points_vector_cord->value;
            curr_cord->next = malloc(sizeof(cord));
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
            points_vector_cord = points_vector_cord->next;
        }

        // last element dont need to allocate new cord
        curr_cord->value = points_vector_cord->value;

        points_vector_vector = points_vector_vector->next;
        points_vector_cord = points_vector_vector->cords;

        head_cord = malloc(sizeof(cord));
        curr_cord = head_cord;
        curr_cord->next = NULL;
        clusters[++i] = head_cord;
        k--;
    }

    return clusters;
}

int num_of_cords_in_cord(cord * c) {
    return 3;
}

cord **create_updated_cluster(cord **clusters, int k, vector *points_vector) {
    cord **updated_clusters;
    cord *head_cord, *curr_cord, *next_cord;
    int *num_of_cords_in_cluster;
    int i, j, min_index, l = num_of_cords_in_cord(points_vector->cords);
    double min_distance = __DBL_MAX__, current_distance;

    updated_clusters = malloc(sizeof(k) * sizeof(cord));
    num_of_cords_in_cluster = malloc(sizeof(int) * k);

    for (i = 0; i < k; i++) {
        head_cord = malloc(sizeof(cord));
        curr_cord = head_cord;
        curr_cord->next = NULL;
        
        for (j = 0; j < l - 1; j++) {
            curr_cord->value = 0;
            curr_cord->next = malloc(sizeof(cord));
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
        }
        curr_cord->value = 0;

        updated_clusters[i] = head_cord;
        num_of_cords_in_cluster[i] = 0;
    }

    while (points_vector->next != NULL) {
        for (i = 0; i < k; i++) {
            current_distance = calc_distance(clusters[i], points_vector->cords);
            if (current_distance < min_distance) {
                min_index = i;
                min_distance = current_distance;
            }
        }

        add_point_to_cluster(points_vector->cords, updated_clusters[min_index], l);
        num_of_cords_in_cluster[min_index]++;

        points_vector = points_vector->next;
    }

    return normalize_updated_cluster(updated_clusters, num_of_cords_in_cluster);
}

int main(int argc, char *argv[]){
    vector *pointsVector;
    cord **clusters;
    cord **updated_clusters;
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

    clusters = initializeKCenter(k, pointsVector);
    // printCordsArray(kCenterVector) -> to implement
    
    while (maxOfIter > 0) {
        updated_clusters = create_updated_cluster(clusters, k, pointsVector);
        if (check_epsilon_value(clusters, updated_clusters, EPSILON)) {
            clusters = updated_clusters;
            break;
        }

        clusters = updated_clusters;
    }

    // printCordsArray(clusters) -> to implement
    return 0;
}