
#ifndef THINKER_H
#define THINKER_H

#include <signal.h>
#include <stdio.h>
#include "shareddataaccess/boarddataaccess.h"
#include "shareddataaccess/gamedataaccess.h"
#include "process/process.h"

int initThinkerOnce(BoardSHM* boardSHM, GameDataSHM* gameSHM);
void deinitThinker();

#endif
