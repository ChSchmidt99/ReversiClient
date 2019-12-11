
#ifndef THINKER_H
#define THINKER_H

#include <signal.h>
#include <stdio.h>
#include "shareddataaccess/boarddataaccess.h"
#include "shareddataaccess/gamedataaccess.h"
#include "info/process.h"

int startThinker(BoardSHM* boardSHM, GameDataSHM* gameSHM, ProcessInfo* procInfo);

#endif
