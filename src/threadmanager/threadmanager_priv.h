#include "threadmanager/threadmanager.h"

int joinThreadsWithResult(size_t numberOfThreads, pthread_t threads[numberOfThreads], void** resultBuffer);
int joinThreadsWithoutResult(size_t numberOfThreads, pthread_t threads[numberOfThreads]);