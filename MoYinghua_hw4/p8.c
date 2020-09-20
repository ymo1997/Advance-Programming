#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_ORIGIN 100
#define MAX_LEN 1024

typedef struct Node_{
    void* data;
    struct Node_* next;

}Node;

typedef struct List_
{
    Node* head;
    Node* last;

}List;

typedef struct{
	double x;
	double y;
	double z;
	double v_x;
	double v_y;
	double v_z;
	int state;
} Particle;

typedef struct{
	int age;
	char name[64];
	double gpa;
} Student;


Particle** dataParticle;
Student** dataStudent;
Particle** afterParticle;
Student** afterStudent;
List* dictionary;
int nbins;
typedef int (*cmpStudent)(void *, void *);
typedef int (*cmpParticle)(void *, void *);

typedef unsigned int (*hStudent)(const void*);
typedef unsigned int (*hParticle)(const void*);


//generate the random data
//in order to generate more same data,
//I would like to reduce the possible kinds of value that can be produced.
void generateParticle() {
	dataParticle = (Particle**)malloc(sizeof(Particle*) * NUM_ORIGIN);
	for(int i = 0; i < NUM_ORIGIN; i++) {
		double x = (double)(rand() % 3) / 10;
		double y = (double)(rand() % 3) / 10;
		double z = (double)(rand() % 3) / 10;
		double v_x = (double)(rand() % 3) / 10;
		double v_y = (double)(rand() % 3) / 10;
		double v_z = (double)(rand() % 3) / 10;
		int state = rand() % 5;
		Particle* newParticle = (Particle*)malloc(sizeof(Particle));
		newParticle->x = x;
		newParticle->y = y;
		newParticle->z = z;
		newParticle->v_x = v_x;
		newParticle->v_y = v_y;
		newParticle->v_z = v_z;
		newParticle->state = state;
		dataParticle[i] = newParticle;
	}
}

void generateStudent() {
	dataStudent = (Student**)malloc(sizeof(Student*) * NUM_ORIGIN);
	for(int i = 0; i < NUM_ORIGIN; i++) {
		int age = (rand() % 5) + 20;
		char name[64];
		bzero(name, 64);
		if(rand() % 2 == 1) sprintf(name, "%c", rand() % 26 + 65);
		else sprintf(name, "%c%c", rand() % 26 + 65, rand() % 26 + 65);
		double gpa = ((double)(rand() % 3) / 10 + 3.5);
		Student* newStudent = (Student*)malloc(sizeof(Student));
		newStudent->age = age;
		memcpy(newStudent->name, name, 64);
		newStudent->gpa = gpa;
		dataStudent[i] = newStudent;
	}
}

//compare functions
int compareStudent(Student* a, Student* b) {
	if(a->age != b->age) return 0;
	if(a->gpa != b->gpa) return 0;
	if(strcmp(a->name, b->name) != 0) return 0;
	return 1;
}
int compareParticle(Particle* a, Particle* b) {
	if(a->x != b->x) return 0;
	if(a->y != b->y) return 0;
	if(a->z != b->z) return 0;
	if(a->v_x != b->v_x) return 0;
	if(a->v_y != b->v_y) return 0;
	if(a->v_z != b->v_z) return 0;
	if(a->state != b->state) return 0;
	return 1;
}

//hash_methods
unsigned int hashParticle(const Particle* p) {
	int n = 7;
	int h = p->x + 2 * n * p->y + 3 * n * p->z + 4 * n * p->v_x + 5 * n * p->v_y + 6 * n * p->v_z;
	h += p->state * n * n;
	return h % nbins;
}

unsigned int hashStudent(const Student* s) {
	unsigned int h = 5381;
    int c;
    char* temp = (char*)s->name;
    while((c = *temp++))
        h = 33 * h + c;
	h += (s->age * c + s->gpa * 10);
	return h % nbins;
}

void printParticle(Particle** p, int num) {
	for(int i = 0; i < num; i++) {
		if(p[i] == NULL) {
			printf("%d Duplicate!\n", i);
			continue;
		}
		printf("%d\tPosition: (%.2lf, %.2lf, %.2lf) Velocity: (%.2lf, %.2lf, %.2lf) State: %d\n",
			i, p[i]->x, p[i]->y, p[i]->z, p[i]->v_x, p[i]->v_y, p[i]->v_z, p[i]->state);
	}
}

void printfStudent(Student** s, int num) {
	for(int i = 0; i < num; i++) {
		if(s[i] == NULL) {
			printf("%d Duplicate!\n", i);
			continue;
		}
		printf("%d\tAge: %d GPA: %2.lf Name: %s\n",
			i, s[i]->age, s[i]->gpa, s[i]->name);
	}
}

int uniq(void ** f, int n, int sz, int (*equals)(void *, void *), unsigned int (*h)(const void *key)){
	nbins = 16;
	dictionary = (List*)malloc(sizeof(List) * nbins);
	int result = 0;
	for(int i = 0; i < n; i++) {
		int hashCode = h(f[i]);
		Node* current = dictionary[hashCode].head;
	    while(current) {
	        if(equals(current->data, f[i]) != 0) break;
	        else current = current->next;
	    }
	    if(current == NULL) {
	    	Node* previousPtr = dictionary[hashCode].last;
    		Node* newNode = (Node*)malloc(sizeof(Node));
    		newNode->next = NULL;
    		newNode->data = f[i];
	    	result++;
	    	if(previousPtr == NULL) {
		        dictionary[hashCode].head = newNode;
		    }
		    else{
		        previousPtr->next = newNode;
		    }
		    dictionary[hashCode].last = newNode;

	    }
	    else{
	    	free(f[i]);
	    	f[i] = NULL;
	    }

	}
	return result;
}

void freeAll() {
	for(int i = 0; i < NUM_ORIGIN; i++) {
		free(dataParticle[i]);
		free(dataStudent[i]);
	}
	free(dataParticle);
	free(dataStudent);
	for(int i = 0; i < nbins; i++) {
        Node* current = dictionary[i].head;
        Node* next;

        while(current){
            next = current->next;
            free(current);
            current = next;
        }
    }

    free(dictionary);
}

int main() {
	//Particles
	generateParticle();
	printf("-----------Original-------------\n");
	printParticle(dataParticle, NUM_ORIGIN);
	int left = uniq((void**)dataParticle, NUM_ORIGIN, sizeof(Particle), (cmpParticle)compareParticle, (hParticle)hashParticle);
	printf("-----------Filtered:%d-------------\n", left);
	printParticle(dataParticle, NUM_ORIGIN);

	//Students
	generateStudent();
	printf("-----------Original-------------\n");
	printfStudent(dataStudent, NUM_ORIGIN);
	left = uniq((void**)dataStudent, NUM_ORIGIN, sizeof(Student), (cmpStudent)compareStudent, (hStudent)hashStudent);
	printf("-----------Filtered:%d-------------\n", left);
	printfStudent(dataStudent, NUM_ORIGIN);
	freeAll();

}