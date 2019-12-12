#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include "misc/utilities.h"

typedef struct _ConfigParams {
    char* hostName;
    char* portNumber;
    char* gameKind;
} ConfigParams;

ConfigParams* newParams(char* hostname, char* portNumber, char* gameKind);
void freeConfigParams(ConfigParams* params);

FILE* openFile(char* path);
ConfigParams* getParamsFromFile(char *filePath);

#endif