#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include<time.h>

void rowOriented(int**, int[], int);
void columnOriented(int**, int[], int);

int main(int argc, char** argv) {
    srand(time(NULL));
    int xLength = 4, yLength = 4, integerCap = 50;
    int **array = malloc(xLength*yLength*sizeof(int));
    for(int i = 0; i < xLength*yLength; ++i) {
        array[i] = rand() % integerCap;
    }

    for(int i = 0; i < xLength*yLength; ++i) {
        printf("%d, ", array[i]);
    }
    return 0;
}

void rowOriented(int** A, int b[], int n){
    int row, column;
    int x[] = malloc(n * sizeof(int));
    
    for(row = n-1; row >= 0; --row) {
        x[row] = b[row];

        for(column = row + 1; column < n; ++column) {
            x[row] -= A[row][column] * x[column];
        }

        x[row] /= A[row][row];
    }
}

void columnOriented(int **A, int b[], int n){
    int row, column;
    int x[] = malloc(n * sizeof(int));

    for(row = 0; row < n; ++row) {
        x[row] = b[row];
    }

    for(column = n-1; column >= 0; --column){
        x[column] /= A[column][column];

        for(row = 0; row < column; ++row) {
            x[row] -= A[row][column] * x[column];
        }
    }
}