#include "misc/config.h"
#include <string.h>

#define NUMBER_OF_STRING 3
#define FILEPATH_BUFFER_SIZE 256
#define MAX_STRING_SIZE 100

ConfigParams* newParams(char* hostname, char* portNumber, char* gameKind){
    ConfigParams* params = safeMalloc(sizeof(ConfigParams));
    params->hostName = copyStringToNewMemoryAddr(hostname);
    params->portNumber = copyStringToNewMemoryAddr(portNumber);
    params->gameKind = copyStringToNewMemoryAddr(gameKind);
    return params;
}

void freeConfigParams(ConfigParams* params){
    free(params->gameKind);
    free(params->portNumber);
    free(params->hostName);
    free(params);
}

FILE* openFile(char* path) {
    FILE *file = NULL;
    if ((file=fopen(path,"r")) == NULL) { 
        printf("Fehler beim Oeffnen der Config-Datei!\n");
    }
  
    return file;
}

ConfigParams* getParamsFromFile(char *filePath) {
  
  FILE* configFile = openFile(filePath);
  char ch;
  int i = 0;
  char paramsArray[NUMBER_OF_STRING][MAX_STRING_SIZE];
  char key[100];
  char keyValue[100];
  char *pointer = key;
  while ((ch = getc(configFile)) != EOF) {
      if (ch == '\t' || ch == ' ') continue;
      else if ( ch == '=') {
        i = 0;
        pointer = keyValue; 
        continue;
      }
      else if ( ch == '\n') {
        i = 0;
        pointer = key;
        if (strcmp(key, "hostName") == 0){
          strcpy(paramsArray[0], keyValue);
        }
        else if (strcmp(key, "portNumber") == 0){
          strcpy(paramsArray[1], keyValue);
        }
        else if (strcmp(key, "gameKind") == 0){
          strcpy(paramsArray[2], keyValue);
        }
        memset(key,0,sizeof(key));
        memset(keyValue,0,sizeof(keyValue));
        continue;
      }
      pointer[i++] = ch;
  }
  ConfigParams* params = newParams(paramsArray[0], paramsArray[1], paramsArray[2]);
  fclose(configFile);
  return params;
}