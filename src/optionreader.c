#include <stdlib.h>
#include <unistd.h> 
#include "../include/optionreader.h"
#include "../include/utilities.h"

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