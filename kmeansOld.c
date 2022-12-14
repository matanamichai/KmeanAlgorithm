#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

double **dataPoint;
FILE *file;
char *fileName ;
int numOfPoint = 0;
int lengthOfVector = 0;
unsigned long size = 1000;
int numberOfKCenter = 3;
double ***kCenterPointArray;
double **kCenter;
int *numberOfPointInCenter;
int *lastIndexInPointArray;
int numOfIter = 0;
int maxOfIter = 200;
double e = 0.001;
void calNumOfPoint(){
    int first = 1;
    char *string,*tok;
    int bytes_read;
    file = fopen(fileName,"r");
    while (!feof(file))
    {
        string =(char *)calloc(size,sizeof(char));
        bytes_read = getline (&string, &size, file);
        if(bytes_read != -1){
            if(first ){
                size = strlen(string);
                tok = strtok(string,",");
                while (tok != NULL)
                {
                    lengthOfVector++;
                    tok = strtok(NULL, ",");
                }
                
                first = 0;
            }
            numOfPoint += 1;
        }

    }
}
void fillDataPoint(){
    char *string,*tok;
    int i,bytesRead,j;
    file = fopen(fileName,"r");
    for(i=0;i<numOfPoint;i++)
    {
        string =(char*)calloc(size,sizeof(char));
        bytesRead = getline (&string, &size, file);
        if(bytesRead != -1){
        tok = strtok(string,",");
        for(j=0;j<lengthOfVector;j++)
        {
            if(j == lengthOfVector-1){
                tok = strtok(tok,"\n");
            }
            dataPoint[i][j] = strtod(tok,&tok+sizeof(double));
            tok = strtok(NULL,",");
        }
        }

    }
    

}
void pringKCenterPoint(){
    int i,j;
    for(i=0;i<numberOfKCenter;i++){
        for(j=0;j<lengthOfVector;j++){
            printf("%.4f ",kCenter[i][j]);
        }
        printf("\n");
    }
}
void clearNumberOfpointInCenter(){
    int i;
    for(i=0;i<numberOfKCenter;i++){
        numberOfPointInCenter[i] = 0;
        lastIndexInPointArray[i] = 0;
    }
}
void initialDataPoint(){
    int i;
    dataPoint = calloc(numOfPoint,sizeof(double *));
    for(i=0;i<numOfPoint;i++){
        dataPoint[i] = calloc(lengthOfVector,sizeof(double));
    }
    
}
void initializeK(){
    int j,i;
    kCenter = calloc(numberOfKCenter,sizeof(double*));
    kCenterPointArray = calloc(numberOfKCenter,sizeof(double *));
    numberOfPointInCenter = calloc(numberOfKCenter,sizeof(int));
    lastIndexInPointArray = calloc(numberOfKCenter,sizeof(int));
    for(i =0;i<numberOfKCenter;i++){
        kCenter[i] = calloc(lengthOfVector,sizeof(double));
        for(j=0;j<lengthOfVector;j++){
            kCenter[i][j] = dataPoint[i][j];
            
        }
        
    }
    clearNumberOfpointInCenter();
}
void printKcenterPoint(){
    int i,j;
    for(i=0;i<numberOfKCenter;i++){
        for(j=0;j<lengthOfVector;j++){
            printf("%.4f \n",kCenter[i][j]);
        }
    }
}
int distansBetweenNtoK(double *n,double *k){
    int i,sum = 0;
    double dif;
    for(i=0;i<lengthOfVector;i++){
        dif = n[i]-k[i];
        sum += pow(dif,2.0);
    }
    return sum;
}
void calNumberOfpointInCenter(){
    int i,j,minDiffCenter;
    double minDiff,tempDif;
    for(i=0;i<numOfPoint;i++){
        minDiff = __DBL_MAX__;
        minDiffCenter = 0;
        for(j=0;j<numberOfKCenter;j++){
            tempDif = distansBetweenNtoK(dataPoint[i],kCenter[j]);
            if(tempDif<minDiff){
                minDiff =tempDif;
                minDiffCenter = j;
            }
        }
        numberOfPointInCenter[minDiffCenter] += 1;
    }
}
void initializeKPointArray(){
    int i,j;
    for(i=0;i<numberOfKCenter;i++){
        kCenterPointArray[i] = calloc(numberOfPointInCenter[i],sizeof(double *));
        for(j=0;j<numberOfPointInCenter[i];j++){
            kCenterPointArray[i][j] =calloc(lengthOfVector,sizeof(double));
        }
    }
}
void enterPointToArray(double *point,double *center){
    int i;
    for (i = 0; i < lengthOfVector; i++)
    {
        center[i] = point[i];
    }
    
}
double EnterPointsToCenters(){
    double maxDiff,minDiff,tempDif ;
    int i,j,minDiffCenter;

    maxDiff= __DBL_MIN__;
    for(i=0;i<numOfPoint;i++){
        minDiff = __DBL_MAX__;
        minDiffCenter = 0;
        for(j=0;j<numberOfKCenter;j++){
            tempDif = distansBetweenNtoK(dataPoint[i],kCenter[j]);
            if(tempDif<minDiff){
                minDiff =tempDif;
                minDiffCenter = j;
            }
        }
        if(minDiff>maxDiff){
            maxDiff = minDiff;
        }
        enterPointToArray(dataPoint[i],kCenterPointArray[minDiffCenter][lastIndexInPointArray[minDiffCenter]]);
        lastIndexInPointArray[minDiffCenter] += 1;
    }
    return maxDiff;
}
void calNewKCenter(){
    int i,j,k;
    double sum;
    for( i=0;i<numberOfKCenter;i++){
        for( j=0;j<lengthOfVector;j++){
            sum = 0;
            for(k=0;k<numberOfPointInCenter[i];k++){
                sum += kCenterPointArray[i][k][j];
            }
            sum /= numberOfPointInCenter[i];
            kCenter[i][j] = sum;

        }

    }
}
double algoCycle(){
    double maxDiff;
    clearNumberOfpointInCenter();
    calNumberOfpointInCenter();
    initializeKPointArray();
    maxDiff = EnterPointsToCenters();
    calNewKCenter();
    numOfIter += 1;
    return maxDiff;
}
void freeAll(){
    int i,j;
    for(i=0;i<numOfPoint;i++){
        free(dataPoint[i]);
    }
    for(i=0;i<numberOfKCenter;i++){
        for(j=0;j<numberOfPointInCenter[i];j++){
            free(kCenterPointArray[i][j]);
        }
        free(kCenterPointArray[i]);
    }
    
    for(i=0;i<numberOfKCenter;i++){
        free(kCenter[i]);
    }
    free(kCenter);
    free(lastIndexInPointArray);
    free(dataPoint);
    free(numberOfPointInCenter);


}
int main(int argc, char *argv[])
{
    FILE *file;
    char *fileName ;
    if (argc > 3){
    maxOfIter = atoi(argv[2]);
    fileName = argv[3];
    }
    else{
        maxOfIter = 200;
        fileName = argv[2];
    }
    numberOfKCenter = atoi(argv[1]);
    argc ++;
    calNumOfPoint();
    initialDataPoint();
    fillDataPoint();
    initializeK();
    while (1)
    {
        double maxDiff = algoCycle();
        if(maxDiff<e || numOfIter >= maxOfIter){
            break;
        }
    }
    pringKCenterPoint();
    freeAll();
    return 0;
}
