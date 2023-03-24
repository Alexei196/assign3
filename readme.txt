Andrew Rossman
823166078
Due 03/24/23


File Manifest
CountSort/CountSort.c


LinearSystem/LinearSystem.c


readme.txt


Compilation Instructions
gcc CountSort.c -o CountSort -fopenmp
gcc LinearSystem.c -o LinearSystem -fopenmp


Running Instructions
./CountSort <ARRAY_LENGTH> <INTEGER_CAP>
Array Length: Integer Amount of elements allowed inside of the sorted array
Integer Cap: Highest amount of an integer allowed inside of the sort.


./LinearSystem <> <>


Answers to HW Questions
1. (a) The outer loop can keep i, j, and count private as they will be specific to the computation of each thread. Array a, n, and temp will all be shared since they will be accessed by all threads.


(b) No value used in each loop is used in another iteration of the for loop. There are thus no loop dependencies for this loop.


(c) Yes, memcpy works using pointers of where to copy the target memory to. If once were to designate pointers for each index of the array temp and the array a, then one could parallelize memcpy by calling it on the pointers of each index instead of the pointer to the array.


(d) See countSort.c


(e) This serial count sort takes O(n^2) time to compute as it takes n actions for each index in array a. When the program is parallelized on the outer for loop, a thread is allocated for each iteration for a total of n threads. This results in n processes which each run at a time complexity of O(n). Thus, the parallel implementation has a considerable speedup over the serial implementation.


(f) qsort uses quicksort to sort an array using a user-specified comparator. The average performance of quick sort is O(n log n) which is much longer than O(n). The advantage of qsort is that the comparator can be specified when the command is called allowing for various types of data to be used when the command is called, whereas my implementation uses a static greater than comparison on integers.  


2. (a) The outer loop cannot be parallelized because the inner loop relies on previously computed values in x. Should the row = 0 iteration attempt computation while the row = n-1 iteration has not completed, it will attempt to multiply using x[n-1], an undefined number.


(b) The inner loop can be parallelized because the indexes from array x used have already been computed by the serialized outer loop. A parallel implementation here only has to restrict a shared accumulator from concurrent update and share the arrays A and x across all threads.


(c) There is a race condition present inside of the outer loop between the division of x[col] and the subtraction of x[row]. Ideally, the inner loop subtracts all required values from x[row] before the outer loop divides it by A[col+(col*n)]. When parallelized however, there is no guarantee that all required subtractions will be made before the division occurs without moving the division outside the loop. Therefore, parallelization is possible but would require modification of the outer loop.  


(d) The inner loop can be parallelized across col threads. Each parallelized run updates only their index for x[row] using the values of A[col+(row*n)] and x[col] that do not change between threads.


(e)(f) see LinearSystem.c


(g) When 10,000 variables are used, it may not be desirable to assign a thread for every iteration using a static schedule due to the massive amount of threads created. Instead, one should opt for a dynamic schedule so that a limited amount of threads are created so that overhead from the threads is minimized while providing adequate processing for all chunks. We choose dynamic over guided because the computation between threads does not change in complexity.


(h) see linearSystem.c

