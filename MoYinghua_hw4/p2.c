#include "min_heap.h"
#include <omp.h>
#define MAX_LEN 1024

//table's listNode structure
typedef struct Node_{
	int v;
	int distance;
	struct Node_* next;
}Node;

typedef struct List_
{
	Node* head;
	Node* last;

}List;

int vNum;
int eNum;

int *processed;
int *shortest;
// int **allShortest;

List* VE;

//Read V E info
void readVE(char* fileName) {
	char line[MAX_LEN];

	FILE *fp;
	fp = fopen(fileName, "r");
	//ignore the line with # at the front
	while(fgets(line, MAX_LEN, fp) != NULL) {
		if(line[0] == '#') continue;
		// fgets(line, MAX_LEN, fp);
		vNum = atoi(strtok(line, " "));
		eNum = atoi(strtok(NULL, " "));
		// printf("%d, %d", vNum, eNum);
		break;
	}

	// allShortest = (int**)malloc(sizeof(int*) * vNum);
	shortest = (int*)malloc(sizeof(int) * vNum);
	processed = (int*)malloc(sizeof(int) * vNum);

	//store the VE information with a hash table
	VE = (List*)malloc(sizeof(List) * vNum);

	//form the table
	while(fgets(line, MAX_LEN, fp) != NULL) {
		// printf("%s", line);
		int current = atoi(strtok(line, " "));
		int next = atoi(strtok(NULL, " "));
		int distance = atoi(strtok(NULL, " "));

		Node* newEdge = (Node*)malloc(sizeof(Node));
		newEdge->v = next;
		newEdge->distance = distance;
		newEdge->next = NULL;
		//if list is empty
		if(VE[current].head == NULL) {
			VE[current].head = newEdge;
		}
		else{
			VE[current].last->next = newEdge;
		}
		VE[current].last = newEdge;
	}
	fclose(fp);

	// // check the table of VE
	// for(int i = 0; i < vNum; i++){
	// 	Node* current = VE[i].head;
	// 	printf("%d ", i);
	// 	while(current) {
	// 		printf("(%d\t%d)\t", current->v, current->distance);
	// 		current = current->next;
			
	// 	}
	// 	printf("\n");
	// }
}

//get the shortest for a single node
void dijkstra_single_src(int source) {
	// allShortest = (int**)malloc(sizeof(int*) * vNum);
	// shortest = (int*)malloc(sizeof(int) * vNum);
	//initialize the processed as -1 and a heap;
	int finish = 0;
	memset(shortest, 1, sizeof(int) * vNum);
	memset(processed, 1, sizeof(int) * vNum);

	// for(int i = 0; i <vNum; i++) printf("%d\t%d\n", i, shortest[i]);

	//insert source node 
	MinHeap h = heapInitialize(eNum);
	push(h, source, 0);
	//while not empty
	while(finish <= vNum && isEmpty(h) != 1) {
		//get the shortest
		HeapNode current = pop(h);
		if(processed[current.v] == 0) continue;
		processed[current.v] = 0;
		shortest[current.v] = current.distance;
		int currentDistance = current.distance;
		Node* currList = VE[current.v].head;
		//update the value in list by pushing new value
		while(currList) {
			int update = currentDistance + currList->distance;
			if(update < shortest[currList->v]) {
				shortest[currList->v] = update;
				push(h, currList->v, update);
			}
			currList = currList->next;
		}
		finish++;
	}

	destory(h);
	// for(int i = 0; i <vNum; i++) printf("%d\t%d\n", i, shortest[i]);
	// allShortest[source] = memcpy(allShortest[source], shortest, vNum);

}

//free

void freeAll() {
	//free the memory
	// free(allShortest);
	free(shortest);
	free(processed);

	for(int i = 0; i < vNum; i++) {
		Node* current = VE[i].head;
		Node* next;

		while(current){
			next = current->next;
			free(current);
			current = next;
		}
	}

	free(VE);

}

void dijkstra_all_src() {
	//get the results starting from every node;
	for(int i = 0; i < vNum; i++) {
		dijkstra_single_src(i);
	}
}

int main() {
	char graph1[] = "graph1.txt";
	char graph2[] = "graph2.txt";
	// char graph3[] = "graph3.txt";
	printf("Please Note the graph2 may run for 55 mins\n");
	double start, end;
	start = omp_get_wtime();
	readVE(graph1);
	dijkstra_all_src();
	end = omp_get_wtime();
	printf("graph1 dijkstra_all_src() runtime: %lf\n", end - start);
	freeAll();

	start = omp_get_wtime();
	readVE(graph2);
	dijkstra_all_src();
	end = omp_get_wtime();
	printf("graph2 dijkstra_all_src() runtime: %lf\n", end - start);
	freeAll();

}