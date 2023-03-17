#include<stdio.h>
#include<stdlib.h>

#ifdef _OPENMP
    #include<omp.h>
#endif    

void HelloWorld(void);

int main(int argc, char **argv) {
    int threadCount = strtol(argv[1], NULL, 10);

    #pragma omp parallel num_threads(threadCount)
    HelloWorld();

    return 0; 
}

void HelloWorld(void ) {
    #ifdef _OPENMP
        int rank = omp_get_thread_num();
        int threadCount = omp_get_num_threads();
    #else
        int rank = 1;
        int threadCount = 1;
    #endif    

    printf("Hello from thread %d of %d\n", rank, threadCount);
}