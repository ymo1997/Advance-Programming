#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MinData -1
typedef struct HeapNode_
{
	int v;
	int distance;

}HeapNode;

typedef struct Heap{
	HeapNode *p;
	int size;
	int capacity;
} *MinHeap;

MinHeap initialize(int maxSize);
int isFull(MinHeap H);
int isEmpty(MinHeap H);
void push(MinHeap H, int source, int value);
HeapNode pop(MinHeap H);
void destory(MinHeap H);

//initialize
MinHeap heapInitialize(int maxSize) {
	MinHeap H = (MinHeap)malloc(sizeof(struct Heap));
	H->p = (HeapNode*)malloc((maxSize + 1) * sizeof(HeapNode));
	memset(H->p, 1, sizeof((maxSize + 1) * sizeof(HeapNode)));
	HeapNode newNode;
	newNode.v = MinData;
	newNode.distance = MinData;
	H->p[0] = newNode;
	H->size = 0;
	H->capacity = maxSize;
	return H;
}

int isFull(MinHeap H) {
	return (H->size == H->capacity) ? 1 : 0;
}

int isEmpty(MinHeap H) {
	return (H->size == 0) ? 1 : 0;
}

//push new value into the heap
void push(MinHeap H, int source, int value) {
	int i = 0;
	if(isFull(H) == 0) {
		for(i = ++H->size; value < H->p[i / 2].distance; i /= 2) {
			H->p[i] = H->p[i / 2];
		}
	}
	HeapNode newNode;
	newNode.v = source;
	newNode.distance = value;
	H->p[i] = newNode;
}

//pop the top from the heap
HeapNode pop(MinHeap H) {
	HeapNode minValue, lastValue;
	int child, parent;
	minValue = H->p[1];
	lastValue = H->p[H->size--];
	for(parent = 1; 2 * parent <= H->size; parent = child) {
		child = 2 * parent;
		if(child != H->size && H->p[child + 1].distance < H->p[child].distance) {
			child++;
		}
		if(lastValue.distance < H->p[child].distance) break;
		else H->p[parent] = H->p[child];
	}
	H->p[parent] = lastValue;
	return minValue;
}

//free memory
void destory(MinHeap H) {
	free(H->p);
	free(H);
}
