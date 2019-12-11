#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include "misc/utilities.h"

typedef struct _Params {
    char* hostName;
    char* portNumber;
    char* gameKind;
} Params;

Params* newParams(char* hostName, char* portNumber, char* gameKind);
void freeParams(Params* params);

FILE* openFile(char* path);
Params* getParamsFromFile(char *filePath);

#endif