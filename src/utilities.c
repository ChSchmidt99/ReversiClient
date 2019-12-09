#include "utilities.h"
#include <string.h>

void panic (char* message) {
    perror(message);
    exit (EXIT_FAILURE);
}

char* copyStringToNewMemoryAddr(const char* str){
    
    char* newString = malloc((strlen(str) + 1) * sizeof(char));    
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

void printStringWithTerminator(const char* str){
    int length = strlen(str);
    for (int i = 0; i <= length; i++){
        if (str[i] == '\0')
            printf("NULLTERM");
        else         
            printf("%c",str[i]);
    }
    printf("\n");
}

void freeArrayWithContents(void** arr, size_t length){
    for (size_t i = 0; i < length; i++){
        free(arr[i]);
    }
    free(arr);
}
    
char** slice(char* str, char *delimiter, size_t* lengthOut) {
    
    char* copy = copyStringToNewMemoryAddr(str);

    char** result = NULL;
    char* token = strtok(copy, delimiter);

    *lengthOut = 0;

    while(token) {
        result = realloc(result, sizeof(char*) * ++(*lengthOut));

        if(result == NULL) 
            panic("Cannot realloc to split string to char**\n");
        

        result[*lengthOut - 1] = token;

        token = strtok(NULL, delimiter);
    }
    return result;
}

void freeTokens(char** tokens){
    free(tokens[0]);
    free(tokens);
}

char* joinTokens(char** tokens, size_t length, char* delimiter){
    size_t newLength = 1;
    for (size_t i = 0; i < length; i++){
        newLength += strlen(tokens[i]) + strlen(delimiter);
    }
    newLength -= strlen(delimiter);
    char* out = malloc(sizeof(char) * newLength);
    for (size_t i = 0; i < length; i++){
        strcat(out, tokens[i]);
        if (i  != length - 1)
            strcat(out, delimiter);
    }
    return out;
}