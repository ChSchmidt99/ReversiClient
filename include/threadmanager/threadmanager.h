#ifndef THREADMANAGER_H
#define THREADMANAGER_H
    #include <pthread.h>
    #include <stdlib.h>
    int startMultipleThreads(size_t numberOfThreads, pthread_t* threadIdsBuffer,void *(*func)(void *), void *args);
    int startThread(pthread_t *threadIdBuffer,void *(*func)(void *), void *args);
    int joinThreads(size_t numberOfThreads, pthread_t threads[numberOfThreads], void* resultBuffer[numberOfThreads]);
#endif