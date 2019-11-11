#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include "../include/optionreader.h"

char* copyStringToNewMemoryAddr(char* str){
    char* newString = malloc(sizeof(char) * (strlen(str) + 1));
    if (newString == NULL){
        perror("Failed to allocate memory");
    }
    strcpy(newString,str);
    return newString;
}

char* getOptionArg(int argc, char *argv[], const char option){
    char optStr[2] = {option,':'};
    char opt;
    while((opt = getopt(argc, argv, optStr)) != -1) {
        if (opt == option) {
            return copyStringToNewMemoryAddr(optarg);
        }
    } 
    return NULL;
}

char* readGameID(int argc, char *argv[]){
     return getOptionArg(argc,argv,'g');
}

char* readPreferencedPlayerNumber(int argc, char *argv[]){
    return getOptionArg(argc,argv,'p');
}