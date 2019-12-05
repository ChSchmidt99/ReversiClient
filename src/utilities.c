#include "utilities.h"
#include <string.h>

void panic (char* message) {
    perror(message);
    exit (EXIT_FAILURE);
}

char* copyStringToNewMemoryAddr(const char* str){
    char* newString = malloc(sizeof(char) * (strlen(str) + 1));
    if (newString == NULL){
        perror("Failed to allocate memory");
    }
    strcpy(newString,str);
    return newString;
}

char* concatStringToNewMemoryAddr(const char* str1, const char* str2, const char* divider){
    char* out = calloc((strlen(str1) + strlen(str2) + strlen(divider) + 1), sizeof(char));
    strcat(out, str1);
    strcat(out, divider);
    strcat(out, str2);
    return out;
}

char** slice(const char* str, char delimiter[], int limit) {
    char* copy = copyStringToNewMemoryAddr(str);
    printf("slicing string '%s' with delimiter '%s'\n", copy, delimiter);
    char* token = strtok(copy, delimiter);
    int delimiterC = 0;
    char** result = NULL;
    if(limit == -1) {}

    unsigned long length = strlen(str);
    unsigned long index = 0;
    if(limit == -1) {
        while(token) {
            result = realloc(result, sizeof(char*) * ++delimiterC);

            if(result == NULL) {
                panic("Cannot realloc to split string to char**\n");
                exit(EXIT_FAILURE);
            }
            result[delimiterC - 1] = token;
            index += strlen(token) + 1;
            token = strtok(NULL, delimiter);
        }
    } else {
        while(token && --limit > 0) {
            result = realloc(result, sizeof(char*) * ++delimiterC);

            if(result == NULL) {
                panic("Cannot realloc to split string to char**\n");
                exit(EXIT_FAILURE);
            }
            result[delimiterC - 1] = token;
            printf("length of '%s' is %lu\n", token, strlen(token));
            index += strlen(token) + 1;
            token = strtok(NULL, delimiter);
        }
        printf("%lu > %lu > %i\n", length, index, limit);
        result = realloc(result, sizeof(char*) * (delimiterC + 1));
        if(token) {
            printf("copying rest..\n");
            char* pointer = result[delimiterC];
            for(unsigned long l = index; l < length; l++) {
                printf("%c", str[l]);
                pointer[l] = str[l];
            }
            pointer[length] = 0;
        } else {
            result[delimiterC] = 0;
        }
    }

    result = realloc(result, sizeof(char*) * (delimiterC + 1));
    if(result == NULL) {
        panic("Cannot realloc to split string to char**\n");
        exit (EXIT_FAILURE);
    }
    result[delimiterC] = 0;
    //printf("%s ----- >>>>>\n", strtok(NULL, delimiter));
    //free(copy);

    return result;
}

int sliceLength(char** data) {
    int length = 0;
    while(data[length++] != 0) {}

    return length - 1;
}