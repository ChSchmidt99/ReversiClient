#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include "misc/utilities.h"
#include "core.h"

FILE* openFile(char* path);
int setParamsFromFile(InputParams* params, char *filePath);
#endif