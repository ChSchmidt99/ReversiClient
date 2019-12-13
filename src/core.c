#include "core.h"
#include "misc/utilities.h"
#include <stdlib.h>

PlayerMeta* newPlayerMeta(int number, char* name, int isReady){
    PlayerMeta* meta = safeMalloc(sizeof(PlayerMeta));
    meta->name = copyStringToNewMemoryAddr(name);
    meta->number = number;
    meta->isReady = isReady;
    return meta;
}

void freePlayerMeta(PlayerMeta* meta){
    free(meta->name);
    free(meta);
}

char* gameKindString(GameKind gameKind){
    if (gameKind == gamekind_Reversi)
        return copyStringToNewMemoryAddr("Reversi");
    else 
        return copyStringToNewMemoryAddr("Unknown");
}