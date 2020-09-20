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

// global variable
int wordNum;
List* dictionary;
int binsNum;
unsigned int (*hashFunction)(char*, int);

int add_word(char * word, char * definition);

unsigned int naive_hash(char * word, int nbins)
{
    unsigned int h = 0;
    int c;
    while((c = *word++))
        h += c;
    return h % nbins;
}
unsigned int bernstein_hash(char * word, int nbins)
{
    unsigned int h = 5381;
    int c;
    while((c = *word++))
        h = 33 * h + c;
    return h % nbins;
}
unsigned int FNV_hash(char * word, int nbins)
{
    unsigned long h = 14695981039346656037lu;
    char c;
    while((c = *word++))
    {
        h = h * 1099511628211lu;
        h = h ^ c;
    }
    return h % nbins;
}

double occupancy() {
    return (double)wordNum / (double)binsNum;
}

void clear_dictionary()
{
    for(int i = 0; i < binsNum; i++) {
        Node* current = dictionary[i].head;
        Node* next;

        while(current){
            next = current->next;
            free(current);
            current = next;
        }
    }

    free(dictionary);

    binsNum = 16;
    wordNum = 0;
    dictionary = (List*)malloc(binsNum * sizeof(List));

    for(int i = 0; i < binsNum; i++) dictionary[i].head = dictionary[i].last = NULL;
}

void resize_dictionary() {
    char** tempWords = (char**)malloc(sizeof(char*) * wordNum);
    char** tempDefinition = (char**)malloc(sizeof(char*) * wordNum);
    //go over all word
    int count = 0;
    int tempBinsNum = binsNum;
    for(int i = 0; i < binsNum; i++) {
        Node* current = dictionary[i].head;
        while(current){
            tempWords[count] = (char*)malloc(sizeof(char) * WORD_LEN);
            tempDefinition[count] = (char*)malloc(sizeof(char) * DEF_LEN);
            memcpy(tempWords[count], current->word, WORD_LEN);
            memcpy(tempDefinition[count], current->definition, DEF_LEN);

            count++;
            current = current->next;
        }
    }

    clear_dictionary();
    free(dictionary);

    binsNum = tempBinsNum;
    binsNum *= 2;
    dictionary = (List*)malloc(binsNum * sizeof(List));
    for(int i = 0; i < binsNum; i++) dictionary[i].head = dictionary[i].last = NULL;

    for(int i = 0; i < count; i++) {
        unsigned int hashCode = hashFunction(tempWords[i], binsNum);

        Node* previousPtr = dictionary[hashCode].last;
        Node* newNode = (Node*)malloc(sizeof(Node));
        memcpy(newNode->word, tempWords[i], WORD_LEN);
        memcpy(newNode->definition, tempDefinition[i], DEF_LEN);
        newNode->next = NULL;

        if(previousPtr == NULL) {
            dictionary[hashCode].head = newNode;
        }
        else{
            previousPtr->next = newNode;
        }
        dictionary[hashCode].last = newNode;
        wordNum++;
    }

    for(int i = 0; i < wordNum; i++) {
        free(tempWords[i]);
        free(tempDefinition[i]);
    }
    free(tempWords);
    free(tempDefinition);

}

int add_word(char * word, char * definition)
{
    // printf("%d %s %s\n", wordNum, word, definition);
    double occupied = occupancy();
    if(occupied > OCCUPANCY_THRESHOLD) {
    // printf("Before resize: binsNum:%d, wordNum: %d \n", binsNum, wordNum);
        resize_dictionary();
    // printf("After resize: binsNum:%d, wordNum: %d \n", binsNum, wordNum);
    }

    unsigned int hashCode = hashFunction(word, binsNum);

    Node* current = dictionary[hashCode].head;
    while(current) {
        if(strncmp(word, current->word, WORD_LEN) == 0) {
            printf("A duplicate word Added.\n");
            return 1;
        }
        current = current->next;
    }

    Node* previousPtr = dictionary[hashCode].last;
    Node* newNode = (Node*)malloc(sizeof(Node));
    memcpy(newNode->word, word, WORD_LEN);
    memcpy(newNode->definition, definition, DEF_LEN);
    // printf("%d %s %s\n", wordNum, word, definition);

    newNode->next = NULL;

    if(previousPtr == NULL) {
        dictionary[hashCode].head = newNode;
    }
    else{
        previousPtr->next = newNode;
    }
    dictionary[hashCode].last = newNode;
    wordNum++;
    return 0;
}

int find_word(char * word)
{
    unsigned int hashCode = (*hashFunction)(word, binsNum);

    Node* current = dictionary[hashCode].head;
    while(current) {
        if(strncmp(word, current->word, WORD_LEN) == 0) {
            printf("Word Found!\n");
            printf("%s %s\n", current->word, current->definition);
            return 0;
        }
        current = current->next;
    }
    return 1;
}

int delete_word(char * word)
{
    int hashCode = (*hashFunction)(word, binsNum);
    Node* current = dictionary[hashCode].head;
    if(current == NULL) {
        return 1;
    }

    if(strncmp(word, current->word, WORD_LEN) == 0) {
        dictionary[hashCode].head = current->next;
        if(current == dictionary[hashCode].last) {
            dictionary[hashCode].last = NULL;
        }
        free(current);
        wordNum--;
        return 0;
    }

    while(current->next) {
        if(strncmp(word, current->next->word, WORD_LEN) == 0) {
            Node* temp = current->next;
            current->next = temp->next;
            if(temp == dictionary[hashCode].last){
                dictionary[hashCode].last = current;
            }
            free(temp);
            wordNum--;

            return 0;
        }
        current = current->next;
    }
    return 1;
}

void swap(char** tempWords, char** tempDefinition, int x, int y) {
    char* wordTemp = tempWords[x];
    char* definitionTemp = tempDefinition[x];
    tempWords[x] = tempWords[y];
    tempDefinition[x] = tempDefinition[y];
    tempWords[y] = wordTemp;
    tempDefinition[y] = definitionTemp;
}

void quickSort(char** tempWords, char** tempDefinition, int left, int right) {
    if(left >= right) return;
    int pos = left + 1;
    for(int i = left + 1; i <= right; i++){
        if(strncmp(tempWords[i], tempWords[left], WORD_LEN) < 0) {
            swap(tempWords, tempDefinition, pos, i);
            pos++;
        }
    }
    pos--;
    swap(tempWords, tempDefinition, pos, left);
    quickSort(tempWords, tempDefinition, left, pos - 1);
    quickSort(tempWords, tempDefinition, pos + 1, right);

}

void print_dictionary()
{
    char** tempWords = (char**)malloc(sizeof(char*) * wordNum);
    char** tempDefinition = (char**)malloc(sizeof(char*) * wordNum);
    //go over all word
    int count = 0;
    for(int i = 0; i < binsNum; i++) {
        Node* current = dictionary[i].head;
        while(current){
            tempWords[count] = current->word;
            tempDefinition[count] = current->definition;
            count++;
            current = current->next;
        }
    }

    //sorting quick sort
    quickSort(tempWords, tempDefinition, 0, wordNum - 1);

    //print
    for(int i = 0; i < wordNum; i++) {
        printf("%s\t%s\n", tempWords[i], tempDefinition[i]);
    }

    free(tempWords);
    free(tempDefinition);
}

void quit() {
    free(dictionary);
}

int import_file(char * fname)
{
    FILE *content = fopen(fname, "r");
    char buff[LINE_LEN];
    if(content == NULL) return 1;
    while(NULL != fgets(buff, LINE_LEN, content)){
        char word[WORD_LEN];
        bzero(word, WORD_LEN);
        char definition[DEF_LEN];
        bzero(definition, DEF_LEN);

        char* spacePos = strstr(buff, " ");
        char* startPos = strstr(buff, "\"");
        char* endPos = strrchr(buff, '\"');
        int wordLen = spacePos - buff;
        int definitionLen = endPos - startPos;
        strncpy(word, buff, wordLen);
        strncpy(definition, startPos, definitionLen + 1);

        add_word(word, definition);
    }

    fclose(content);
    return 0;
}

void stats() {
    double occupied = occupancy();
    int usedBin = 0;
    int maxNum = 0;

    for(int i = 0; i < binsNum; i++) {
        Node* current = dictionary[i].head;
        if(current) usedBin++;
        int tempCount = 0;
        while(current){
            tempCount++;
            current = current->next;
        }
        maxNum = maxNum > tempCount ? maxNum : tempCount;
    }
    double usedBinFrac = (double)usedBin / binsNum;
    printf("Bins:%d\noccupancy:%lf\nused bin fraction:%lf\nmax entries in a bin:%d\nwordNum:%d\n", binsNum, occupied, usedBinFrac, maxNum, wordNum);
}

int main(int argc, char** argv)
{
    int hashMethodNum = atoi(argv[1]);

    switch(hashMethodNum) {
        case(1):
            hashFunction = naive_hash;
            break;
        case(2):
            hashFunction = bernstein_hash;
            break;
        case(3):
            hashFunction = FNV_hash;
            break;
        default:
            printf("Wrong Hash Method Number!\n");
            return 0;
    }

    binsNum = 16;
    wordNum = 0;
    dictionary = (List*)malloc(binsNum * sizeof(List));
    for(int i = 0; i < binsNum; i++) dictionary[i].head = dictionary[i].last = NULL;


    while(1)
    {
        int error;
        char command[COMMAND_LEN];

        /* Get the User's Command */
        printf(">$ ");
        if(scanf("%s", command)) {};

        /* Add word */
        if( strncmp(command, "add", COMMAND_LEN) == 0 )
        {
            char word[WORD_LEN];
            char definition[DEF_LEN];
            char * pos;
            if(scanf("%s ",word)) {};
            /* Gets the rest of the input */
            if(fgets(definition, DEF_LEN, stdin)) {};
            if ((pos=strchr(definition, '\n')) != NULL) *pos = '\0';
            /* Adds to dictionary */
            error = add_word(word, definition);

            if( error == 0 )
                printf("The word \"%s\" has been added successfully.\n", word);
            else
                printf("Error when adding word \"%s\".\n", word);

        }
        /* Import file */
        if( strncmp(command, "import", COMMAND_LEN) == 0 )
        {
            char fname[DEF_LEN];
            if(scanf("%s",fname)) {};
            error = import_file(fname);
            if( error == 0 )
                printf("The file \"%s\" has been imported successfully.\n", fname);
            else
                printf("Error when adding file \"%s\".\n", fname);
        }
        /* Delete word */
        if( strncmp(command, "delete", COMMAND_LEN) == 0 )
        {
            char word[WORD_LEN];
            if(scanf("%s",word)) {};
            error = delete_word(word);
            if( error == 0 )
                printf("The word \"%s\" has been deleted successfully.\n", word);
            else
                printf("Error when deleting word \"%s\".\n", word);
        }
        /* Find word */
        if( strncmp(command, "find", COMMAND_LEN) == 0 )
        {
            char word[WORD_LEN];
            if(scanf("%s",word)) {};
            error = find_word(word);
            if( error == 1 )
                printf("Error when finding word \"%s\".\n", word);
        }
        /* Print dictionary */
        if( strncmp(command, "print", COMMAND_LEN) == 0 )
            print_dictionary();
        /* clear dictionary */
        if( strncmp(command, "clear", COMMAND_LEN) == 0 ){
            clear_dictionary();
            printf("Clear Successfully\n");

        }
        if(strncmp(command, "stats", COMMAND_LEN) == 0) {
            stats();
        }
        /* quit program */
        if( strncmp(command, "quit", COMMAND_LEN) == 0 ){
            quit();
            break;
        }

    }
    return 0;
}

