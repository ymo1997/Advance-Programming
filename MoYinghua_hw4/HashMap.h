#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN 32
#define DEF_LEN 1024
#define COMMAND_LEN 32
#define LINE_LEN 1024
#define OCCUPANCY_THRESHOLD 0.75


typedef struct Node_{
    char word[WORD_LEN];
    char definition[DEF_LEN];
    struct Node_* next;

}Node;

typedef struct List_
{
    Node* head;
    Node* last;

}List;