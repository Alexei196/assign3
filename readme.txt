Andrew Rossman
823166078
Due 03/24/23

File Manifest
CountSort/CountSort.c

LinearSystem/LinearSystem.c

readme.txt

Compliation Instructions
gcc CountSort.c -o CountSort -fopenmp
gcc LinearSystem.c -o LinearSystem -fopenmp

Running Instructions
./CountSort <> <>
./LinearSystem <> <>

Answers to HW Questions
1. (a) The outer loop can keep i, j, and count private as they will be specific to the computation of each thread. Array a, n, and temp will all be shared since they will be accessed by all threads, and temp will need to have update protection so that threads do not overwire each other while accessing temp. 

(b) No value used in each loop is used in another iteration of the for loop. There are thus no loop dependencies for this loop. 

(c) Yes, memcpy works using pointers of where to copy the target memory to. If once were to designate pointers for each index of the array temp and the array a, then one could parallelize memcpy by calling it on the pointers of each index instead of the pointer to the array.

(d) See countSort.c

(e) This serial count sort takes O(n^2) time to compute as it takes n actions for each index in array a. When the program is parallelized on the outer for loop, a thread is allocated for each interation for a total of n threads. This results in n processes which each run at a time complexity of O(n). Thus, the parallel implementation has a considerable speedup over the serial implementation. 

(f) qsort uses quick sort to sort an array using a user specified comparator. the average performance of quick sort is O(n log n) which is much longer than O(n). The advantage of qsort is that the comparator can be specified when the command is called allowing for various types of data to be used when the command is called, whereas my implementation uses a static greater than comparison on integers.  

2. (a) The outer loop cannot be parallelized because the inner loop relies on previously computed values in x. Should the row = 0 iteration attempt computation while the row = n-1 iteration has not completed, it will attempt to multiply using x[n-1], an currently undefined number. 

(b) The inner loop can be parallelized because the indexes from array x used have already been computed by the serialized outer loop. A parallel implementation here only has to restrict x[row] from concurrent update and share array A across all threads. 