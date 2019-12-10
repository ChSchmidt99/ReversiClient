#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <stdio.h>

void panic(char* message);
char* copyStringToNewMemoryAddr(const char* str);
char* concatStringToNewMemoryAddr(const char* str1, const char* str2, const char* divider);

char** slice(const char* str, char *delimiter, size_t* lengthOut, int limit);
int sliceLength(char** data);

#endif