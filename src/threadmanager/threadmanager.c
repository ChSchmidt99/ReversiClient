#include "threadmanager_priv.h"
#include "misc/utilities.h"
#include <stdio.h>

int startMultipleThreads(size_t numberOfThreads, pthread_t* threadIdsBuffer,void *(*func)(void *), void *args){
    for (size_t i = 0; i < numberOfThreads; i++){
        if (pthread_create(&threadIdsBuffer[i], NULL, func, args) != 0)
            return -1;
    }
    return 0;
}

int startThread(pthread_t *threadIdBuffer,void *(*func)(void *), void *args){
    if (pthread_create(threadIdBuffer, NULL, func, args) != 0)
        return -1;
    else 
        return 0;
}

int joinThreads(size_t numberOfThreads, pthread_t threads[numberOfThreads], void* resultBuffer[numberOfThreads]){
    if (resultBuffer == NULL)
        return joinThreadsWithoutResult(numberOfThreads,threads);
    else 
        return joinThreadsWithResult(numberOfThreads,threads,resultBuffer);
}

int joinThreadsWithResult(size_t numberOfThreads, pthread_t threads[numberOfThreads], void** resultBuffer){
    for (size_t i = 0; i < numberOfThreads; i++){
        if (pthread_join(threads[i], &resultBuffer[i]) != 0)
            return -1;
    }
    return 0;
}

int joinThreadsWithoutResult(size_t numberOfThreads, pthread_t threads[numberOfThreads]){
    for (size_t i = 0; i < numberOfThreads; i++){
        if (pthread_join(threads[i], NULL) != 0)
            return -1;
    }
    return 0;
}