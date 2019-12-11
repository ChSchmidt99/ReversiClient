
#ifndef THINKER_H
#define THINKER_H

#include <signal.h>
#include <stdio.h>
#include "shareddataaccess/shareddataaccess.h"
#include "info/process.h"

void tick(SharedMemory* shm, ProcessInfo* procInfo);

#endif
