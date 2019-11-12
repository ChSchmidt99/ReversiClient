#include "../include/utilities.h"
#include <string.h>

void die (char* message) {
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
    char* out = malloc(sizeof(char) * (strlen(str1) + strlen(str2) + strlen(divider)));
    strcat(out, str1);
    strcat(out, divider);
    strcat(out, str2);
    return out;
}