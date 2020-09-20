#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<math.h>

void heapify(int* f, int n);
int* allocate_array(int argc, char* argv[]);

void swap(int *f, int a, int b){ /*swap two variables' value;*/
	int temp = f[a];
	f[a] = f[b];
	f[b] = temp;
}

void heapify(int* f, int n){
	int currPos = n - 1; 
	int maxPos;
	if(currPos % 2 == 1){ /* if the tree has one leaf that has only one child;*/
		swap(f, n - 1, (n - 1) / 2);
		currPos -= 1;
	}
	for(; currPos > 0; currPos -= 2){/* from buttom to the top, for each node and its sister and its parent, we compare and swap.*/
		maxPos = f[currPos] > f[currPos - 1] ? currPos : currPos - 1;
		maxPos = f[maxPos] > f[(currPos - 1) / 2] ? maxPos : (currPos - 1) / 2;
		swap(f, maxPos, (currPos - 1) / 2);/* find the max value of these three node and swap with the parent;*/

	}

}

int* allocate_array(int argc, char* argv[]) {
	int i = 1;
	int arrSize = argc - 1; /*size of the array*/
	int *arr;/* array*/
	arr = (int*)malloc(sizeof(int)*arrSize);/* allocate the space*/

	for(; i < argc; i++){
		arr[i - 1] = atoi(argv[i]);/*transform char to int*/
	}
	for(i = 0; i < log(arrSize) / log(2); i++) heapify(arr, arrSize);/* heapify for logn times*/
	return arr;
}

int main(int argc, char* argv[]){
	int *arr = allocate_array(argc, argv);
	int j = 0;
	for(; j < argc - 1; j++){
		printf("%d ", arr[j]);/*print out result.*/
	}
	printf("\n");
	free(arr);
}

