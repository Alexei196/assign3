#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<omp.h>

void Serial_Count_Sort(int[], int);
void Parallel_Count_Sort(int[], int);

int main(int argc, char** argv) {
    int lengthOfArray = 100, integerCap = 100;
    if(argc > 1) {
        if(atoi(argv[1]) == 0) {
            fprintf(stderr, "Cannot use provided Length");
        }
        lengthOfArray = atoi(argv[1]);
    }
    if(argc > 2) {
        integerCap = atoi(argv[2]);
    }
    srand(time(NULL));
    

    int *numberArray = malloc(lengthOfArray * sizeof(int));
    printf("Unsorted Array: ");
    for(int num = 0; num < lengthOfArray; ++num) {
        numberArray[num] = rand() % integerCap;
        printf("%d, ", numberArray[num]);
    }
    printf("\n");

    Parallel_Count_Sort(numberArray, lengthOfArray);

    printf("Sorted Array: ");
    for(int num = 0; num < 100; ++num) {
        printf("%d, ", numberArray[num]);
    }

    printf("\n");
    free(numberArray);
}

void Serial_Count_sort(int a[], int n) {
    int i, j, count;
    int *temp = malloc(n * sizeof(int));

    for(i = 0; i < n; ++i) {
        count = 0;

        for(j =0; j < n; ++j) {
            if(a[j] < a[i]) ++count;
            else if(a[j] == a[i] && j < i) ++count;
        }
        temp[count] = a[i];
    }

    memcpy(a, temp, n*sizeof(int));
    free(temp);
}

void Parallel_Count_Sort(int a[], int n) {
    int i, j, count = 0;
    int *temp = malloc(n * sizeof(int));

    #pragma omp parallel for num_threads(n) \
    default(none) shared(a, temp, n) private(i, j, count)
    for(i = 0; i < n; ++i) {
        count = 0;

        for(j = 0; j < n; ++j) {
            if(a[j] < a[i]) ++count;
            else if(a[j] == a[i] && j < i) ++count;
        }
        temp[count] = a[i];
    }

    memcpy(a, temp, n*sizeof(int));
    free(temp);
}