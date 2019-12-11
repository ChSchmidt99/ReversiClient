#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <stdio.h>

#define LOG_LEVEL 1
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

void panic(char* message);
char* copyStringToNewMemoryAddr(const char* str);
char* concatStringToNewMemoryAddr(const char* str1, const char* str2, const char* divider);    
double fRand(double fMin, double fMax);
int getRandomInt(int min, int max);
void printStringWithTerminator(const char* str);
char* newStringWithoutDelimiter(const char* str, char delimiter);
char** slice(const char* str, char *delimiter, size_t* lengthOut);
char** sliceLimit(const char* str, char *delimiter, size_t* lengthOut, int limit);
void freeTokens(char** tokens);
char* joinTokens(char** tokens, size_t length, char* delimiter);
void freeArrayWithContents(void** arr, size_t length);
void logMessage(char* message, int level);

#endif