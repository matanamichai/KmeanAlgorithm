#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    double value;
    struct cord *next;
} cord;

typedef struct 
{
    struct vector *next;
    struct cord *cords;
} vector;

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



int main(int argc, char *argv[]){
    struct vector *headVector;
    int maxOfIter;
    if (argc > 2){
        maxOfIter = atoi(argv[2]);
    }
    else{
        maxOfIter = 200;
    }
    
    headVector = fillDataPoint();
}