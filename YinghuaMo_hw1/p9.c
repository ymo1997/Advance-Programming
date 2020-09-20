#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct{
	double* data; /* Pointer to data */	
	int size; /* Size of the array from the user's perspective */
	int capacity; /* The actual size of allocation "data" pointer points to */
} Vector;
/* Sets the element at given index of the vector to value */
void set(Vector* v, int index, double value){
	v->data[index] = value;/*set the value*/
}
/* Returns the value at given index of the vector */
double get(Vector* v, int index){
	return v->data[index];/*return value*/
}

/* Doubles the capacity of the vector */
/* If capacity is 0, increases capacity to 1 */ /* Used by the "insert" and "append" functions */ 
void resize(Vector* v){
	double* newData = (double*)malloc(sizeof(double) * (v->capacity * 2 + 1)); /*ask for bigger space*/
	double* temp = v->data;
	int i;
	for(i = 0; i < v->size; i++){
		newData[i] = temp[i];/*copy data to the new vector*/
	}
	v->data = newData;/*replace the original data*/
	v->capacity = 2 * v->capacity + 1;
	free(temp);/*free the original space*/
}
/* Moves all values at and after the given index to the right */ /* one element, then sets the value at given index. */
void insert(Vector* v, int index, double value){
	int i;
	if(v->size == v->capacity) resize(v);
	for(i = v->size; i > index; i--){
		v->data[i] = v->data[i - 1];
	}
	v->data[i] = value;
	v->size++;
}
/* Appends a value to the vector, automatically */
/* resizing the vector if necessary by calling the */ /* resize() function */
void append(Vector* v, double value){
	if(v->size == v->capacity) resize(v);
	v->data[v->size] = value;
	v->size++;
}
/* Deletes the given index, and moves all values after */
/* the index to the left by one element. */
/* This reduces the size of the vector by 1, but the */
/* capacity should remain the same */
void delete( Vector* v, int index){
	int i;
	for(i = index + 1; i <= v->size; i++){
		v->data[i - 1] = v->data[i];
	}
	v->size--;
}
/* Initializes the vector to a specified size. */
/* Capacity is set to be equal to the size of the vector. */ /* All elements of the vector are initialized to 0. */
void init(Vector* v, int size){
	v->data = (double*)malloc(sizeof(double) * size);
	memset(v->data, 0, sizeof(double) * size);
	v->size = size;
	v->capacity = size;
}
/* Free's any memory allocated to data pointed of the vector, */ /* sets size = capacity = 0, and the data pointer to NULL. */ 
void free_vector(Vector* v){
	free(v->data);
	v->size = 0;/*reset size to 0*/
	v->capacity = 0;/*reset capacity to zero*/
}
/* Prints the vector in a clean format. If vector is empty, */ /* just print "< >" */
void print(Vector* v){
	int i;
	printf("< ");
	for(i = 0; i < v->size; i++){
		if(i == v->size - 1) printf("%.1f", v->data[i]);
		else printf("%.1f, ", v->data[i]);
	}
	printf(" >\n");
}

int main(){
	double i;
	Vector* v = (Vector*)malloc(sizeof(Vector));
	printf("test init...\n");
	init(v, 2);
	print(v);
	printf("test append...\n");
	for(i = 0; i < 5; i++){
		append(v, i);
		print(v);
	}
	printf("test set...\n");
	set(v, 3, 0.1);
	print(v);
	set(v, 0, 2.0);
	print(v);
	printf("test get...\n");
	printf("get value = %.1f\n", get(v, 3));
	print(v);
	printf("test insert...\n");
	insert(v, 2, 2.0);
	print(v);
    insert(v, 5, 3.0);
	print(v);
	printf("test delete...\n");
    delete(v, 6);
	print(v);
	printf("test append...\n");
    for(i = 0.0; i < 1.0; i+= 0.2){
    	append(v, i);
		print(v);

    }
	printf("test free...\n");
    free_vector(v);
	print(v);


}
