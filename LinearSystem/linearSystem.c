#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include<time.h>

int* rowOriented(int*, int*, int);
int* columnOriented(int*, int*, int);

int main(int argc, char** argv) {
    srand(time(NULL));
    int xLength = 4, yLength = 4, integerCap = 50;
    int *array = malloc(xLength*yLength*sizeof(int));
    for(int i = 0; i < xLength*yLength; ++i) {
        array[i] = rand() % integerCap;
    }

    for(int i = 0; i < xLength*yLength; ++i) {
        if(i % xLength == 0 && i != 0) printf("\n");
        printf("%d, ", (int) array[i]);
        
    }
    //Array A has a 1 dimension x by y matrix

    printf("\n");
    free(array);
    return 0;
}

int* rowOriented(int *A, int *b, int n){
    int row, column;
    int x[n];

    for(row = n-1; row >= 0; --row) {
        x[row] = b[row];

        for(column = row + 1; column < n; ++column) {
            x[row] -= A[row*column] * x[column];
        }

        x[row] /= A[row*row];
    }
    return x;
}

int* columnOriented(int *A, int *b, int n){
    int row, column;
    int x[n];

    for(row = 0; row < n; ++row) {
        x[row] = b[row];
    }

    for(column = n-1; column >= 0; --column){
        x[column] /= A[column*column];

        for(row = 0; row < column; ++row) {
            x[row] -= A[row*column] * x[column];
        }
    }

    return x;
}