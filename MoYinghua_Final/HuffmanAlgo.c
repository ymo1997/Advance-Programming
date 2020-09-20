#include "huffman_tree.h"
#define FILENAME_LEN 64
#define FILE_LEN 1024
#define ASCII_NUM 128
#define STREAM_LEN 100000

//global variables
int* frequency;
char** code;
char* encodedStream;
char* decodedStream;
HuffmanNode* huffmanRoot;
MinHeap heap;
int imported;

// function flow : count frequency -> heap -> huffman tree
int import_file(char* fileName) {
    // count frequency
    FILE *content = fopen(fileName, "r");
    char current;
    frequency = (int*)malloc(ASCII_NUM * sizeof(int));
    memset(frequency, 0, (ASCII_NUM * sizeof(int)));
    if(content == NULL) return 1;
    while((current = fgetc(content)) != EOF) {
        frequency[(int)current]++;
    }
    fclose(content);

    //generate heap
    int count = 0;
    heap = heapInitialize(ASCII_NUM);
    for(int i = 0; i < ASCII_NUM; i++) {
        if(frequency[i] == 0) continue;
        // if(i < 32 && frequency <= 0) continue;
        HuffmanNode* newNode = (HuffmanNode*)malloc(sizeof(HuffmanNode));
        count++;
//        printf("%d %c %d\n", count, (char)i, frequency[i]);
        newNode->character = (char)i;
        newNode->weight = frequency[i];
        newNode->parent = NULL;
        newNode->left = NULL;
        newNode->right = NULL;

        push(heap, newNode);
    }

    //generate huffman tree
    huffmanRoot = huffmanInitialize(heap);

    // get and store the code
    code = (char**)malloc(sizeof(char*) * ASCII_NUM);
    for(int i = 0; i < ASCII_NUM; i++) code[i] = NULL;
    huffmanTraverse(code, huffmanRoot);
    imported = 1;
    return 0;
}

int encode(char* fileName) {
    // if have not imported file to generate huffman code
    if(imported == 0) return 1;
    encodedStream = (char*)malloc(sizeof(char) * STREAM_LEN);
    FILE *content = fopen(fileName, "r");
    if(content == NULL) return 1;
    char current;
    int result = 0;
    //get every character and concat the huffman code
    while((current = fgetc(content)) != EOF){
        //if imported file doesn't have the corresponding character, then return failing message
        if(frequency[(int)current] == 0) {
            printf("File contains character that hasn't been encoded by importing.\n");
            return 1;
        }
        int codeLen = strlen(code[(int)current]);
        result += codeLen;
        strncat(encodedStream, code[(int)current], codeLen);
    }
    //output the 01 stream
    printf("%s\n", encodedStream);
    printf("\nNumber of bits: %d\n", result);
    fclose(content);
    free(encodedStream);
    return 0;
}

int decode(char* fileName) {
    FILE *content = fopen(fileName, "r");
    if(content == NULL) return 1;

    char c;
    decodedStream = (char*)malloc(sizeof(char) * STREAM_LEN);
    memset(decodedStream, 0, sizeof(char) * STREAM_LEN);
    HuffmanNode* current = huffmanRoot;
    //get every bit and traverse the tree
    //print out a character when meet with a leaf
    while((c = fgetc(content)) != EOF) {
        if(c != '0' && c != '1') return 1;
        if(c == '0') {
            current = current->left;
        }
        else{
            current = current->right;
        }
        if(current->left == NULL && current->right == NULL) {
            // printf("\npos:%d\t character:%c\n", pos, current->character);
            strncat(decodedStream, &current->character, 1);
            current = huffmanRoot;
        }
    }
    printf("%s\n", decodedStream);
    fclose(content);
    free(decodedStream);
    return 0;
}

void dump() {
    //print out characters and corresponding huffman code
    for(int i = 0; i < ASCII_NUM; i++) {
        if(code[i] == NULL) continue;
        printf("%c;%s\n", (char)i, code[i]);
    }
}

void quit() {
    // prevent memory leak before quit the program
    free(frequency);
    for(int i = 0; i < ASCII_NUM; i++) {
        if(code[i] == NULL) continue;
        free(code[i]);
    }
    free(code);
    destory(heap, huffmanRoot);
}

int main(int argc, char** argv)
{
    imported = 0;
    while(1)
    {
        int error;
        char command[FILENAME_LEN];

        // get the command
        printf(">$ ");
        if(scanf("%s", command)) {};

        // import file
        if(strncmp(command, "import", FILENAME_LEN) == 0)
        {
            char fileName[FILENAME_LEN];
            if(scanf("%s",fileName)) {};
            error = import_file(fileName);
            if(error == 0)printf("The file \"%s\" has been imported successfully.\n", fileName);
            else printf("Error when importing file \"%s\".\n", fileName);
        }

        //encode file
        if(strncmp(command, "encode", FILENAME_LEN) == 0)
        {
            char fileName[FILENAME_LEN];
            if(scanf("%s",fileName)) {};
            error = encode(fileName);
            if(error == 0) printf("The file \"%s\" has been encoded successfully.\n", fileName);
            else printf("Error when encoding file \"%s\".\n", fileName);
        }

        //decode file
        if( strncmp(command, "decode", FILENAME_LEN) == 0 )
        {
            char fileName[FILENAME_LEN];
            if(scanf("%s",fileName)) {};
            error = decode(fileName);
            if(error == 0) printf("The word \"%s\" has been decoded successfully.\n", fileName);
            else printf("Error when decoding word \"%s\".\n", fileName);
        }

        // dump
        if( strncmp(command, "dump", FILENAME_LEN) == 0 ) {
            dump();
        }

        // quit
        if( strncmp(command, "quit", FILENAME_LEN) == 0 ){
            if(imported == 1) quit();
            break;
        }

    }
    return 0;
}