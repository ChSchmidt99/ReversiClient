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

PlayerMeta* copyPlayerMetaToNewAddr(PlayerMeta* playerMeta){
    return newPlayerMeta(playerMeta->number, playerMeta->name, playerMeta->isReady);
}

void freePlayerMeta(PlayerMeta* meta){
    free(meta->name);
    free(meta);
}

ProcessInfo* newProcessInfo(int pipe[2], pid_t thinkerPID, pid_t communicatorPID){
    ProcessInfo* info = safeMalloc(sizeof(ProcessInfo));
    info->fd[0] = pipe[0];
    info->fd[1] = pipe[1];
    info->thinkerPID = thinkerPID;
    info->communicatorPID = communicatorPID;
    return info;
}

void freeProcessInfo(ProcessInfo* procInfo) {
    free(procInfo);
}

char* gameKindString(GameKind gameKind){
    if (gameKind == gamekind_Reversi)
        return copyStringToNewMemoryAddr("Reversi");
    else 
        return copyStringToNewMemoryAddr("Unknown");
}