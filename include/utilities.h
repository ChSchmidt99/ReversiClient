#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <stdio.h>

void panic(char* message);
char* copyStringToNewMemoryAddr(const char* str);
char* concatStringToNewMemoryAddr(const char* str1, const char* str2, const char* divider);

void printStringWithTerminator(const char* str);
char** slice(char* str, char *delimiter, size_t* lengthOut);
void freeTokens(char** tokens);
char* joinTokens(char** tokens, size_t length, char* delimiter);
void freeArrayWithContents(void** arr, size_t length);

#endif