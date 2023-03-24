#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<omp.h>
#include<sys/resource.h>

void Serial_Count_Sort(int[], int);
void Parallel_Count_Sort(int[], int);

int threadCount = 8;
//.out <ARRAY_LENGTH> <THREAD_COUNT> <INTEGER_CAP> 
int main(int argc, char** argv) {
    int lengthOfArray = 100, integerCap = 100;
    //Handle inputs and default values.
    if(argc > 1) {
        if(atoi(argv[1]) <= 0) {
            fprintf(stderr, "Cannot use provided Array Length\nDefaulting to %d\n", lengthOfArray);
        }else {
            lengthOfArray = atoi(argv[1]);
        }
    }
    if(argc > 2) {
        if(atoi(argv[2]) <= 0) {
            fprintf(stderr, "Cannot use provided Thread Count\nDefaulting to %d\n", threadCount);
        }else {
            threadCount = atoi(argv[2]);
        }
    }
    if(argc > 3) {
        if(atoi(argv[3]) <= 0) {
            fprintf(stderr, "Cannot use provided Integer Cap\nDefaulting to %d\n", integerCap);
        } else {
            integerCap = atoi(argv[3]);
        }
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
    //Compute resource usage just after sorting occurs.  
    struct rusage resourceManager;
    getrusage(RUSAGE_SELF, &resourceManager);

    printf("Sorted Array: ");
    for(int num = 0; num < lengthOfArray; ++num) {
        printf("%d, ", numberArray[num]);
    }
    printf("\n");
    printf("My System time is %ld.%06ld\n", resourceManager.ru_stime.tv_sec, resourceManager.ru_stime.tv_usec);
    printf("My User time is %ld.%06ld\n", resourceManager.ru_utime.tv_sec, resourceManager.ru_utime.tv_usec);

    free(numberArray);
    return 0;
}
//Serial Count sort provided by prompt
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
//Parallel count sort with dynamic scheduled fixed amount threads 
void Parallel_Count_Sort(int a[], int n) {
    int i, j, count = 0;
    int *temp = malloc(n * sizeof(int));

    #pragma omp parallel for num_threads(threadCount) \
    schedule(dynamic) default(none) shared(a, temp, n) private(i, j, count)
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