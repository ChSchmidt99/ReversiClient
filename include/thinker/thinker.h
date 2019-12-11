
#ifndef THINKER_H
#define THINKER_H

#include <signal.h>
#include <stdio.h>
#include "shareddataaccess/boarddataaccess.h"
#include "info/process.h"

void tick(BoardSHM* boardSHM, ProcessInfo* procInfo);

#endif
