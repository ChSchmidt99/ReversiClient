#include "misc/config.h"
#include <string.h>

#define NUMBER_OF_STRING 3
#define FILEPATH_BUFFER_SIZE 256
#define MAX_STRING_SIZE 100

FILE* openFile(char* path) {
    FILE *file = NULL;
    if ((file=fopen(path,"r")) == NULL) { 
        printf("Fehler beim Oeffnen der Config-Datei!\n");
    }
  
    return file;
}

int setParamsFromFile(InputParams* params, char *filePath){
  FILE* configFile = openFile(filePath);
  if (configFile == NULL)
    return -1;

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
  params->hostName = copyStringToNewMemoryAddr(paramsArray[0]);
  printf("%s\n", params->hostName);
  params->portNumber = copyStringToNewMemoryAddr(paramsArray[1]);
  params->gameKind = copyStringToNewMemoryAddr(paramsArray[2]);
  fclose(configFile);
  return 0;
}