#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include<time.h>
#include<string.h>
#include<sys/resource.h>
#define RANDOMDOUBLE (double) (rand() % (integerCap*2)) - (integerCap);

enum solutionRequired {ELIMINATION='e', SUBSTITUTION='s'};

void rowOriented(double*, double*, double*,int);
void columnOriented(double*, double*, double*, int);
void guassElimination(double*, double*, int);
void displayArray(double*, double*, int);

int threadCount = 8;

int main(int argc, char** argv) { 
    char substitutionMethod = 'r', matrixType = 'r', solutionRequired;
    int n = 4, integerCap = 20;
    //start at default, then change to specified to command line specified options
    /*Introducing Option based command calls!
    Options are: 
    -h, --help: Display usage of this program. Terminates immediately after discovery.
    -s, --substitution: Specify whether to use 'r' or 'c' backwards propagation
    -c, --number_cap: Specifies the highest/lowest possible number positive/negative to simplify computation
    -t, --thread_count: Specify the amount of threads 
    -n, --matrix_size: Specify the desired size of the matrix used for elimination 
    -m, --matrix_type: Specify the use of full random matrix, triangular random matrix, or a predefined identity matrix
    */
    for(int i = 1; i < argc; ++i) {
        if(strcmp("--help", argv[i]) == 0 || strcmp("-h", argv[i]) == 0) {     
            printf("Usage: LinearSystem <OPTIONS> <SPECIFIC> \nOptions are:\n-h, --help: Display usage of this program. Does not use variable specific\n-s, --substitution: Specify to use row 'r' or column 'c' focused backwards propagation\n-c, --number_cap: Specifies the highest/lowest possible integer positive/negative to simplify computation\n-t, --thread_count: Specify the integer amount of threads\n-n, --matrix_size: Specify the desired integer size of the matrix used for elimination\n-m, --matrix_type: Specify the use of full random matrix 'r', triangular random matrix 't', or a predefined identity matrix 'p'.");
            exit(0);
        }
        if(argv[i][0] == '-' && (i+1) >= argc) {
            fprintf(stderr, "Cannot reach argument of %s\n", argv[i]);
            break;
        }
        if(strcmp("--substitution", argv[i]) == 0 || strcmp("-s", argv[i]) == 0) {
            switch(argv[++i][0]) {
                case 'r':
                case 'c':
                case 'R':
                case 'C':
                    substitutionMethod = argv[i][0];
                break; 
                default:
                    fprintf(stderr, "Substitution option %c is not valid! use 'r' or 'c'!\nDefaulting to %c\n", argv[i][0], substitutionMethod);
                break;
            }
            continue;
        }
        if(strcmp("--matrix_type", argv[i]) == 0 || strcmp("-m", argv[i]) == 0) {
            switch(argv[++i][0]) {
                case 'p':
                case 'P':
                case 'r':
                case 'R':
                case 't':
                case 'T':
                    matrixType = argv[i][0];
                break; 
                default:
                    fprintf(stderr, "Matrix Type option %c is not valid! use 'r', 'p' or 't'!\nDefaulting to %c\n", argv[i][0], matrixType);
                break;
            }
            continue;
        }
        if(strcmp("--number_cap", argv[i]) == 0 || strcmp("-c", argv[i]) == 0) {     
            if(atoi(argv[++i]) <= 0) {
                fprintf(stderr, "Integer Cap cannot be zero or below!\nDefaulting to %d", integerCap);
            } else {
                integerCap = atoi(argv[i]);
            }
            continue;
        }
        if(strcmp("--thread_count", argv[i]) == 0 || strcmp("-t", argv[i]) == 0) {
            if(atoi(argv[++i]) <= 0) {
                fprintf(stderr, "Thread Count cannot be zero or below!\nDefaulting to %d", threadCount);
            } else {
                threadCount = atoi(argv[i]);
            }
            continue;
        }
        if(strcmp("--matrix_size", argv[i]) == 0 || strcmp("-n", argv[i]) == 0) {
            if(atoi(argv[++i]) <= 0) {
                fprintf(stderr, "Matrix Size cannot be zero or below!\nDefaulting to %d", n);
            } else {
                n = atoi(argv[i]);
            }
            continue;
        }
    }
    //Allocate arrays after command line passes inspection
    double *array = malloc(n*n*sizeof(double));
    double *solutions = malloc(n * sizeof(double));
    double *rightHandSide = malloc(n * sizeof(double)); 
    srand(time(NULL));
    //Random array generation for the matrix
    //Solution required also specified at this level, since already triangular matrixes will not require guassian elimination 
    switch(matrixType){
        //Predefined
        /* The matrix A and right hand side b are defined as separate by the prompt, but we are asked to compute a given identity matrix, assuming the right most column of the matrix is the right hand array, will result in a no solution matrix.

        To maintain consistency across all matrix types, the predefined matrix A will be treated as separate from b in the algorithm and be given random values of b. This results in an unlikely probalility that the solution array is impossible to find. 
        */
        case 'p':
        case 'P':
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < n; ++j) {
                    if(i == j) {
                        array[j + (i*n)] = -4;
                        continue;
                    }
                    if(i+1 == j || i-1 == j) {
                        array[j + (i*n)] = 1;
                    } else {
                        array[j + (i*n)] = 0;
                    }
                }
            }
            solutionRequired = 'e';
        break;
        //Full Random
        case 'r':
        case 'R':
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < n; ++j) {
                    array[j + (i*n)] = RANDOMDOUBLE;
                }
            }
            solutionRequired = 'e';
        break;
        //Triangular Random
        case 't':
        case 'T':
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < n; ++j) {
                    if(i == j) {
                        array[j + (i*n)] = 1;
                        continue;
                    }
                    if(j > i) {
                        array[j + (i*n)] = RANDOMDOUBLE;
                    } else {
                        array[j + (i*n)] = 0;
                    }
                }
            }
            solutionRequired = 's';
        break; 
    }
    //Init right hand side as random for all matrix types
    for(int i = 0; i < n; ++i) {
        rightHandSide[i] = RANDOMDOUBLE ;
    }

    printf("Before Elimination:");
    displayArray(array, rightHandSide, n);
    //Core of program, all work required by prompt done here
    //Algorithm decides on the solution and then sequentually executes steps until solution is found
    switch(solutionRequired) {
        case ELIMINATION:
            guassElimination(array, rightHandSide, n);
            printf("After Elimination:");
            displayArray(array, rightHandSide, n);
        case SUBSTITUTION:
            switch(substitutionMethod) {
                case 'r':
                case 'R':
                    rowOriented(array, rightHandSide, solutions, n);
                    printf("\nROW");
                break;
                case 'c':
                case 'C':
                    columnOriented(array, rightHandSide, solutions, n);
                    printf("\nCOLUMN");
                break;
            }
        break;
    }
    struct rusage resourceManager;
    getrusage(RUSAGE_SELF, &resourceManager);

    printf(" SOLUTIONS:\n");
    for(int i = 0; i < n; ++i) {
        if(i % n == 0 && i != 0) printf("\n");
        printf("%f, ", (double) solutions[i]);
    }
    printf("\n");
    printf("My System time is %ld.%06ld\n", resourceManager.ru_stime.tv_sec, resourceManager.ru_stime.tv_usec);
    printf("My User time is %ld.%06ld\n", resourceManager.ru_utime.tv_sec, resourceManager.ru_utime.tv_usec);
    //Deallocation
    free(array);
    free(solutions);
    free(rightHandSide);
    return 0;
}
//Substitution by rows
void rowOriented(double *A, double *b, double*x, int n){
    int row, column;
    double sharedSum;
    for(row = n-1; row >= 0; --row) {
        sharedSum = b[row];
        #pragma omp parallel for num_threads(threadCount) \
        schedule(dynamic) reduction(-: sharedSum) shared(A, row, n) private(column)
        for(column = row + 1; column < n; ++column) {
            sharedSum -= A[column+(row*n)] * x[column];
        }
        x[row] = sharedSum / A[row+(row*n)];
    }
}
//Substitution by columns
void columnOriented(double *A, double *b, double* x,  int  n){
    int row, column;
    for(row = 0; row < n; ++row) {
        x[row] = b[row];
    }

    for(column = n-1; column >= 0; --column){
        x[column] /= A[column+(column*n)];

        #pragma omp parallel for num_threads(threadCount) \
        schedule(dynamic) shared(A, n, x, column) private(row)
        for(row = 0; row < column; ++row) {
             x[row] -= A[column+(row*n)] * x[column];
        }
        
    }
}
//Parallel guass elimination, like the substitution algorithms, the outer loops contained dependencies on previous loops making parallelization difficult without changes.   
void guassElimination(double* A, double* b, int n){
    int i, j, t;
    double multiplier;
    
    for(i = 0; i < n; ++i) {
        //There is no remedy for dividing by zero other than altering the array, 
        //the whole program must thus end or restart.
        if(A[i+(i*n)] == 0.0) {
            fprintf(stderr, "\nDivision by zero error in elimination at index [%d, %d]!\nError non-recoverable, exiting...\n", i, i);
            exit(-1);
        }
        #pragma omp parallel for num_threads(threadCount) \
        schedule(dynamic) shared(i, n, A, b) private(multiplier, j, t)
        for(j = i+1; j < n; ++j) {
            multiplier = A[i+(j*n)] / A[i+(i*n)];
            for(t = i; t < n; ++t) {
                A[t+(j*n)] -= multiplier * A[t+(i*n)];
            }
            b[j] -= multiplier * b[i];
        }
    }
}
//Helper method to display the main array in the console with its right hand side.
void displayArray(double* array, double* rightHandSide, int n) {
    printf("\nARRAY: \n");
    for(int i = 0; i < n*n; ++i) {
        if(i % n == 0 && i != 0) printf("\n");
        printf("%f, ", (double) array[i]);
    }
    printf("\nRIGHT HAND SIDE: \n");
    for(int i = 0; i < n; ++i) {
        if(i % n == 0 && i != 0) printf("\n");
        printf("%f, ", (double) rightHandSide[i]);
    }
    printf("\n");
}