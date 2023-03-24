#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include<time.h>

void rowOriented(double*, double*, double*,int);
void columnOriented(double*, double*, double*, int);

int main(int argc, char** argv) {
    srand(time(NULL));
    int n = 4, integerCap = 20;
    double *array = malloc(n*n*sizeof(double));
    double *solutions = malloc(n * sizeof(double));
    double *rightHandSide = malloc(n * sizeof(double)); 

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if(i == j) {
                array[j + (i*n)] = 1;
                continue;
            }
            if(j > i) {
                array[j + (i*n)] = (double) (rand() % integerCap - (integerCap / 2));
            } else {
                array[j + (i*n)] = 0;
            }
        }
    }

    for(int i = 0; i < n; ++i) {
        rightHandSide[i] = (double) (rand() % integerCap) ;
    }

    for(int i = 0; i < n*n; ++i) {
        if(i % n == 0 && i != 0) printf("\n");
        printf("%f, ", (double) array[i]);
        
    }
    
    //Attempt to find Solutions
    
    printf("\nRIGHT HAND SIDE: \n");
    for(int i = 0; i < n; ++i) {
        if(i % n == 0 && i != 0) printf("\n");
        printf("%f, ", (double) rightHandSide[i]);
        
    }
    printf("\n");
    //Row Variant
    rowOriented(array, rightHandSide, solutions, n);
    printf("\nROW RESULTS: \n");
    for(int i = 0; i < n; ++i) {
        if(i % n == 0 && i != 0) printf("\n");
        printf("%f, ", (double) solutions[i]); 
    }
    //Column Variant
    columnOriented(array, rightHandSide, solutions, n);
    printf("\nCOLUMN RESULTS: \n");
    for(int i = 0; i < n; ++i) {
        if(i % n == 0 && i != 0) printf("\n");
        printf("%f, ", (double) solutions[i]);
    }
    printf("\n");

    free(array);
    free(solutions);
    free(rightHandSide);
    return 0;
}

void rowOriented(double *A, double *b, double*x, int n){
    int row, column;
    
    for(row = n-1; row >= 0; --row) {
        x[row] = b[row];
        
        #pragma omp parallel for num_threads(n - (row+1)) \
        shared(A, row, n) private(column)
        for(column = row + 1; column < n; ++column) {
            
            x[row] -= A[column+(row*n)] * x[column];
            printf("\nDEBUG: %f, %d, %d\n", x[row], row, column);
        }

        x[row] /= A[row+(row*n)]; 
    }
}

void columnOriented(double *A, double *b, double* x,  int  n){
    int row, column;

    for(row = 0; row < n; ++row) {
        x[row] = b[row];
    }

    for(column = n-1; column >= 0; --column){
        x[column] /= A[column+(column*n)];

        for(row = 0; row < column; ++row) {
            x[row] -= A[row+(column*n)] * x[column];
        }
    }
}