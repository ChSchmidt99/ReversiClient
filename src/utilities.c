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

int slice(const char* str, char delimiter[], int limit, char** result) {
    char* copy = copyStringToNewMemoryAddr(str);
    printf("slicing string '%s' (old was '%s') with delimiter '%s'\n", copy, str, delimiter);
    char* token = strtok(copy, delimiter);
    int delimiterC = 0;

    while(token && limit-- > 0) {
        result = realloc(result, sizeof(char*) * ++delimiterC);

        if(result == NULL) {
            panic("Cannot realloc to split string to char**\n");
            exit(EXIT_FAILURE);
        }
        printf("%i will be %s\n", delimiterC - 1, token);
        result[delimiterC - 1] = token;

        token = strtok(NULL, delimiter);
    }

    result = realloc(result, sizeof(char*) * (delimiterC + 1));
    result[delimiterC] = 0;
    free(copy);

    return delimiterC;
}

int sliceLength(char** slice) {
    int length = 0;
    while(*slice[length++] != '\0') {}

    return length;
}