#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#define VECTOR_SIZE 10000
typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	int data;
} Tree_node;


/* Adapted Vector type, with data being an array of node pointers */
typedef struct{
	Tree_node ** data; /* Pointer to data */
	int size;      /* Size of the array from the user's perspective */
	int capacity;  /* The actual size of allocation "data" pointer points to */
} Vector;

/* Insert your vector functions here and convert to Tree_node * data type */
/* Insert your vector functions here and convert to Tree_node * data type */
/* Insert your vector functions here and convert to Tree_node * data type */
void init(Vector* v, int size){
	v->data = (Tree_node**)malloc(sizeof(Tree_node*) * size);
	// memset(v->data, 0, sizeof(Tree_node) * size);
	v->size = 0;
	v->capacity = size;
}

void free_vector(Vector* v){
	free(v->data);
	v->size = 0;/*reset size to 0*/
	v->capacity = 0;/*reset capacity to zero*/
	return ;
}

void resize(Vector* v){
	Tree_node** newData = (Tree_node**)malloc(sizeof(Tree_node*) * (v->capacity * 2 + 1)); /*ask for bigger space*/
	Tree_node** temp = v->data;
	int i;
	for(i = 0; i < v->size; i++){
		newData[i] = temp[i];/*copy data to the new vector*/
	}
	v->data = newData;/*replace the original data*/
	v->capacity = 2 * v->capacity + 1;
	free(temp);/*free the original space*/
	return ;
}

void append(Vector* v, Tree_node* value){
	if(v->size == v->capacity) resize(v);
	v->data[v->size] = value;
	v->size++;
	return ;
}

Tree_node* get(Vector* v, int index){
	return v->data[index];/*return value*/
}

int empty(Vector* v){
	if(v->size == 0) return 1;
	else return 0;
}
/* Stack structure built on top of our Vector type */
typedef struct{
	Vector * v;
} Stack;

/* Initializes stack */
Stack * init_stack(void)
{
	Stack *s = (Stack*)malloc(sizeof(Stack));
	s->v = (Vector*)malloc(sizeof(Vector));
	init(s->v, VECTOR_SIZE);
	return s;
}

/* Pushes node pointer onto stack */
void push(Stack * stack, Tree_node * value )
{
	append(stack->v, value);
}

/* Pops node pointer off of stack */
Tree_node * pop( Stack * stack )
{
	Tree_node* result = get(stack->v, stack->v->size - 1);
	stack->v->size--;
	return result;
}


/* Determines if the given tree is a valid BST or not */
int is_tree_iterative(Tree_node * tree)
{
	Stack *s = init_stack();
	Tree_node *current = tree;
	int lastValue = INT_MIN;
	while(current || !empty(s->v)){
		while(current){
			push(s, current);
			current = current->left;
		}
		current = pop(s);
		if(current->data <= lastValue) return 0;
		lastValue = current->data;
		current = current->right;
	}
	free_vector(s->v);
	free(s);
	return 1;
}

Tree_node * build_good_tree(void)
{
	/* Allocate head */
	int n_nodes = 11;
	int i;
	Tree_node * nodes = (Tree_node *) malloc(n_nodes * sizeof(Tree_node));
	for( i = 0; i < n_nodes; i++ )
	{
		nodes[i].data  = i;
		nodes[i].left  = NULL;
		nodes[i].right = NULL;
	}

	/* Top Level */
	nodes[5].left =  &nodes[3];
	nodes[5].right = &nodes[7];
	/* Second Level */
	nodes[3].left =  &nodes[1];
	nodes[3].right = &nodes[4];
	nodes[7].left =  &nodes[6];
	nodes[7].right = &nodes[9];
	/* Third Level */
	nodes[1].left =  &nodes[0];
	nodes[1].right = &nodes[2];
	nodes[9].left =  &nodes[8];
	nodes[9].right = &nodes[10];
	/* Everything else is a leaf */

	return &nodes[5];
}

Tree_node * build_bad_tree(void)
{
	/* Allocate head */
	int n_nodes = 12;
	int i;
	Tree_node * nodes = (Tree_node *) malloc(n_nodes * sizeof(Tree_node));
	for( i = 0; i < n_nodes; i++ )
	{
		nodes[i].data  = i;
		nodes[i].left  = NULL;
		nodes[i].right = NULL;
	}

	/* Top Level */
	nodes[5].left =  &nodes[3];
	nodes[5].right = &nodes[8];
	/* Second Level */
	nodes[3].left =  &nodes[1];
	nodes[3].right = &nodes[6];
	nodes[8].left =  &nodes[4];
	nodes[8].right = &nodes[10];
	/* Third Level */
	nodes[1].left =  &nodes[0];
	nodes[1].right = &nodes[2];
	nodes[6].right = &nodes[9];
	nodes[10].left =  &nodes[7];
	nodes[10].right = &nodes[11];

	return &nodes[5];
}

int main(void)
{
	Tree_node * good_test_tree = build_good_tree();
	Tree_node * bad_test_tree;

	int isBST = is_tree_iterative(good_test_tree);

	if( isBST )
		printf("Tree is valid!\n");
	else
		printf("Tree is invalid!\n");

	bad_test_tree = build_bad_tree();

	isBST = is_tree_iterative(bad_test_tree);

	if( isBST )
		printf("Tree is valid!\n");
	else
		printf("Tree is invalid!\n");


	return 0;
}
