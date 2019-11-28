#include "config.h"
#include <string.h>

#define NUMBER_OF_STRING 3
#define MAX_STRING_SIZE 100

Params* newParams(char* hostname, char* portNumber, char* gameKind){
    Params* params = malloc(sizeof(Params));
    params->hostName = copyStringToNewMemoryAddr(hostname);
    params->portNumber = copyStringToNewMemoryAddr(portNumber);
    params->gameKind = copyStringToNewMemoryAddr(gameKind);
    return params;
}

FILE* openFile(char* path) {
    char filePath[256];
    FILE *file = NULL;
    strncpy(filePath, path, 255*sizeof(char));
        
    if ((file=fopen(filePath,"r")) == NULL) { 
        printf("Fehler beim Oeffnen der Config-Datei!\n");
    }
  
    return file;
}

Params* getParamsFromFile(char *filePath) {
  
  FILE* configFile = openFile(filePath);
  //char line[256];
  char ch;
  int i = 0;

  char paramsArray[NUMBER_OF_STRING][MAX_STRING_SIZE];
  //int j = 0;
  char key[100];
  char keyValue[100];
  char *pointer = key;
  //ich muss params ja erstmal irgendwie belegen oder?
  //Params* params = newParams(keyValue, keyValue, keyValue);
  while ((ch = getc(configFile)) != EOF) {
      if (ch == '\t' || ch == ' ') continue;
      else if ( ch == '=') {
        i = 0;
        pointer = keyValue; 
        continue;
      }
      else if ( ch == '\n') {
        i = 0;
        printf( "KEY %s \n", key);
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
  Params* params = newParams(paramsArray[0], paramsArray[1], paramsArray[2]);
  printf(" params: %s, %s, %s \n", params->hostName, params->portNumber, params->gameKind);
  fclose(configFile);
  return params;
}