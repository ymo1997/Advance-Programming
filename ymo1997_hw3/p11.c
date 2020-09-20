#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

#define TEST_TIME 100000000
int THREADNUM;

void sum_serial(int* randNum, int* result){
	result[0] = randNum[0];
	for(int i = 1; i < TEST_TIME; i++) result[i] = result[i - 1] + randNum[i];
}

void sum_parallel(int* randNum, int* result){
	omp_set_dynamic(0);
	omp_set_num_threads(THREADNUM);
	int* lastVal = (int*)calloc(THREADNUM, sizeof(int));
	#pragma omp parallel
	{
		int id = omp_get_thread_num();
    	int Nthr = omp_get_num_threads();
    	// int taskNumPerThread = TEST_TIME / Nthr;

	    int istart = id * TEST_TIME / Nthr;
	    int iend = (id + 1) * TEST_TIME / Nthr;
	    result[istart] = randNum[istart];
		for(int i = istart + 1; i < iend; i++){
			result[i] = result[i - 1] + randNum[i];
		}
		lastVal[id] = result[iend - 1];
		#pragma omp barrier
		int addition = 0;
		for(int i = 0; i < id; i++){
			addition += lastVal[i];
		}
		for(int i = istart; i < iend; i++){
			result[i] += addition;
		}
	}

}

int compare(int* A, int* B){
	for(int i = 0; i < TEST_TIME; i++){
		if(A[i] != B[i]) return 0;
	}
	return 1;
}

int main(){
	int threads[5] = {1, 2, 4, 8, 16};
	for(int i = 0; i < 5; i++){
		THREADNUM = threads[i];
		printf("THREADNUM: %d \n", THREADNUM);
		int* randNum = (int*)malloc(sizeof(int) * TEST_TIME);
		int* sumA = (int*)calloc(TEST_TIME, sizeof(int));
		int* sumB = (int*)calloc(TEST_TIME, sizeof(int));
		for(int i = 0; i < TEST_TIME; i++) randNum[i] = rand() % 100;
		double start, end;

		start = omp_get_wtime();
		sum_serial(randNum, sumA);
		end = omp_get_wtime();
		printf("sum_serial runtime: %lf \n", end - start);

		start = omp_get_wtime();
		sum_parallel(randNum, sumB);
		end = omp_get_wtime();
		printf("sum_parallel runtime: %lf \n", end - start);

		int result = compare(sumA, sumB);
		printf("result correct ? %d \n", result);

		free(randNum);
		free(sumA);
		free(sumB);
	}
	

}