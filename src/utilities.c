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
    printf("slicing string '%s' (old was '%s') with delimiter '%s'\n", copy, str, delimiter);
    char* token = strtok(copy, delimiter);
    int delimiterC = 0;
    char** result = NULL;
    while(token && (limit == -1 || limit > 0)) {
        result = realloc(result, sizeof(char*) * ++delimiterC);

        if(result == NULL) {
            panic("Cannot realloc to split string to char**\n");
            exit(EXIT_FAILURE);
        }
        printf("%i will be %s\n", delimiterC - 1, token);
        result[delimiterC - 1] = token;
        printf("%i is %s\n", delimiterC - 1, result[delimiterC - 1]);
        if(--limit > 0)
            token = strtok(NULL, delimiter);

        printf("token is %s", token);
    }

    result = realloc(result, sizeof(char*) * (delimiterC + 1));
    if(result == NULL) {
        panic("Cannot realloc to split string to char**\n");
        exit (EXIT_FAILURE);
    }
    result[delimiterC] = token;
    //free(copy);

    return result;
}

int sliceLength(char** data) {
    int length = 0;
    while(data[length++] != 0) {}

    return length;
}