#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

static int n = 2000;
static int nthreadsArr[5] = {1, 2, 4, 8, 16};
static int nthreadsArrSize = 5;
static int **A, **B, **mp, **thread;
static int taskPerThread;

// Computes the 2D matrix product C = AB, with matrix dimensions N x N
// using nthreads number of OpenMP threads
void matrix_product_openmp(int N, int ** C, int ** A, int ** B, int nthreads){
	omp_set_dynamic(0);
	omp_set_num_threads(nthreads);
	#pragma omp parallel
	{
	int id = omp_get_thread_num();
    int Nthr = omp_get_num_threads();

    int istart = id * N / Nthr;
    int iend = (id + 1) * N / Nthr;

	for(int i = istart; i < iend; i++){
		for(int j = 0; j < N; j++){
			C[0][i] += (A[i][j] * B[j][i]);
		}
	}

	}
}

// Computes the 2D matrix product C = AB, with matrix dimensions N x N
// using nthreads number of pthreads

void* helper(void* m){
	int temp = *((int*) m);
	// printf("%d\n", temp);
	int k, l;
	int i = temp * taskPerThread;
	int j = (temp + 1) * taskPerThread;
	// printf("%d %d %d\n", i, j, temp);
	for(k = i; k < j; k++){
		for(l = 0; l < n; l++){
			thread[0][k] += (A[k][l] * B[l][k]);
		}
	}
	return NULL;
}

void matrix_product_pthreads( int N, int ** thread, int ** A, int ** B, int nthreads){
	taskPerThread = N / nthreads;
	// printf("taskPerThread : %d \n", taskPerThread);
	int i;
	int* temp = NULL;
	pthread_t* threads = (pthread_t*)malloc(nthreads * sizeof(pthread_t));
	for(i = 0; i < nthreads; i++){
		// printf("%d\n", i);
		temp = (int*)malloc(sizeof(int));
		*temp = i;
		pthread_create(&threads[i], NULL, &helper, temp);
	}
	for(i = 0; i < nthreads; i++){
		pthread_join(threads[i], NULL);
	}
	free(temp);
	free(threads);
}

int test(int **mp, int** thread, int N){
	int i;
	for(i = 0; i < N; i++){
		if(mp[0][i] != thread[0][i]) return 0;
	}
	return 1;
}

int main(){
	int i, j;
	int testResult;
	double start, end;
	A = (int**)malloc(n * sizeof(int*));
	B = (int**)malloc(n * sizeof(int*));
	mp = (int**)malloc(1 * sizeof(int*));
	mp[0] = (int*)calloc(n, sizeof(int));
	thread = (int**)malloc(1 * sizeof(int*));
	thread[0] = (int*)calloc(n, sizeof(int));

	for(i = 0; i < n; i++){
		A[i] = (int*)malloc(n * sizeof(int));
		B[i] = (int*)malloc(n * sizeof(int));
		for(j = 0; j < n; j++){
			A[i][j] = rand() % 100;
			B[i][j] = rand() % 100;
		}
	}
	for(i = 0; i < nthreadsArrSize; i++){
		int nthreads = nthreadsArr[i];
		printf("#threads : %d \n", nthreads);
		start = omp_get_wtime();
		matrix_product_openmp(n, mp, A, B, nthreads);
		end = omp_get_wtime();
		printf("matrix_product_openmp runtime: %lf\n", end - start);

		start = omp_get_wtime();
		matrix_product_pthreads(n, thread, A, B, nthreads);
		end = omp_get_wtime();
		printf("matrix_product_pthreads runtime: %lf\n", end - start);

		testResult = test(mp, thread, n);
		// for(i = 0; i < n; i++){
		// 	printf("%d \t %d\n", mp[0][i], thread[0][i]);
		// }
		printf("test successfully ? %d\n", testResult);

	}

	free(A);
	free(B);
	free(thread);
	free(mp);
}