#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CODE_LEN 32
#define MinData -1

//structures
typedef struct Huffman {
	char character;
    int weight;
    struct Huffman *left;
    struct Huffman *right;
    struct Huffman *parent;
} HuffmanNode, *HuffmanTree;

typedef struct HeapNode_ {
	HuffmanNode* huffmanPtr;
}HeapNode;

typedef struct Heap{
	HeapNode *p;
	int size;
	int capacity;
}*MinHeap;

MinHeap heapInitialize(int maxSize);
int isFull(MinHeap H);
int isEmpty(MinHeap H);
void push(MinHeap H, HuffmanNode* node);
HeapNode pop(MinHeap H);
void destory(MinHeap H, HuffmanNode* root);

//initialize the heap
MinHeap heapInitialize(int maxSize) {
	MinHeap H = (MinHeap)malloc(sizeof(struct Heap));
	H->p = (HeapNode*)malloc((maxSize + 1) * sizeof(HeapNode));
	memset(H->p, 1, sizeof((maxSize + 1) * sizeof(HeapNode)));
//	for(int i = 0; i <= maxSize; i++) H->p[i] = NULL;
	HeapNode newNode;
	newNode.huffmanPtr = (HuffmanNode*)malloc(sizeof(HuffmanNode));
	newNode.huffmanPtr->character = ' ';
	newNode.huffmanPtr->weight = MinData;
	newNode.huffmanPtr->parent = NULL;
	newNode.huffmanPtr->left = NULL;
    newNode.huffmanPtr->right = NULL;

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
void push(MinHeap H, HuffmanNode* node) {
	int i = 0;
	if(isFull(H) == 0) {
		for(i = ++H->size; node->weight < H->p[i / 2].huffmanPtr->weight; i /= 2) {
			H->p[i] = H->p[i / 2];
		}
	}

	H->p[i].huffmanPtr = (HuffmanNode*)malloc(sizeof(HuffmanNode));
	H->p[i].huffmanPtr = node;
}

//pop the top from the heap
HeapNode pop(MinHeap H) {
	HeapNode minValue, lastValue;
	int child, parent;
	minValue = H->p[1];
	lastValue = H->p[H->size--];
	for(parent = 1; 2 * parent <= H->size; parent = child) {
		child = 2 * parent;
		if(child != H->size && H->p[child + 1].huffmanPtr->weight < H->p[child].huffmanPtr->weight) {
			child++;
		}
		if(lastValue.huffmanPtr->weight < H->p[child].huffmanPtr->weight) break;
		else H->p[parent] = H->p[child];
	}
	H->p[parent] = lastValue;
	return minValue;
}

//free memory
void destoryHelper(HuffmanNode* current) {
	if(current == NULL) return;
	destoryHelper(current->left);
	destoryHelper(current->right);
	free(current);
}
void destory(MinHeap H, HuffmanNode* root) {
	destoryHelper(root);
	free(H->p);
	free(H);
}

//initialize the huffman structure
HuffmanNode* huffmanInitialize (MinHeap H) {
    HuffmanNode *left, *right, *parent;
    int tempSize = H->size;
    for(int i = 0; i < tempSize - 1; i++) {   
    	// get the smallest 2 nodes from heap
        left = pop(H).huffmanPtr;
        right = pop(H).huffmanPtr;
        //printf("Combine: left (%c, %d)\t right (%c, %d)\n", left->character, left->weight, right->character, right->weight);

        // combine 2 nodes into a tree
        HuffmanNode* newNode = (HuffmanNode*)malloc(sizeof(HuffmanNode));
        parent = newNode;
        newNode->character = 0;
        newNode->weight = left->weight + right->weight;
        newNode->left = left;
        newNode->right = right;
        newNode->parent = NULL;

        left->parent = newNode;
        right->parent = newNode;

        // push newNode into heap
        push(H, newNode);
    }
    return parent;
}

void helper(char** code, HuffmanNode* root, char* currentCode, int pos) {
	if(root->left == NULL && root->right == NULL) {
		code[(int)root->character] = (char*)malloc(sizeof(int) * pos);
        // printf("current: %c, currentCode: %s\n", root->character, currentCode);
		memcpy(code[(int)root->character], currentCode, pos);
		return ;
	}
	if(root->left != NULL) {
		currentCode[pos] = '0';
		helper(code, root->left, currentCode, pos + 1);
	}
	if(root->right != NULL) {
		currentCode[pos] = '1';
		helper(code, root->right, currentCode, pos + 1);
	}
}

//generate the the huffman code by traverse the tree recursively.
void huffmanTraverse (char** code, HuffmanNode* root) {
	char* currentCode;
	currentCode = (char*)malloc(sizeof(char) * CODE_LEN);
	memset(currentCode, 0, sizeof(char) * CODE_LEN);

	helper(code, root, currentCode, 0);
	free(currentCode);
}