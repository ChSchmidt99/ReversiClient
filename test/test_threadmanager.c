#include <stdio.h>
#include "misc/utilities.h"
#include "threadmanager/threadmanager.h"

#define TEST_NUMBER_OF_THREADS 5

int threadCalledCount = 0;

void* threadStart(void* num){
    threadCalledCount += (int)num;
    return NULL;
}

void* threadStartWithReturn(void* num){
    return num;
}

static char* test_run_threads(){
    pthread_t threadIDs[TEST_NUMBER_OF_THREADS];
    if (startMultipleThreads(TEST_NUMBER_OF_THREADS,threadIDs,&threadStart,(void*)1) != 0)
        panic("Failed to start threads");

    if (joinThreads(TEST_NUMBER_OF_THREADS, threadIDs, NULL) != 0)
        panic("Failed to join threads");

    mu_assert("Thread function not called correctly", threadCalledCount == TEST_NUMBER_OF_THREADS);
    return 0;
}

static char* test_run_threads_with_return(){
    pthread_t threadIDs[TEST_NUMBER_OF_THREADS];
    int input = 1;
    if (startMultipleThreads(TEST_NUMBER_OF_THREADS,threadIDs,&threadStartWithReturn,INT2VOIDP(input)) != 0)
        panic("Failed to start threads");

    
    void* result[TEST_NUMBER_OF_THREADS];
    if (joinThreads(TEST_NUMBER_OF_THREADS, threadIDs, result) != 0)
        panic("Failed to join threads");

    
    for (size_t i = 0; i < TEST_NUMBER_OF_THREADS; i++){
        mu_assert("Return did not match input", (int)result[i] == input);
    }

    return 0;
}

static char * run_thread_tests() {
    mu_run_test(test_run_threads);
    mu_run_test(test_run_threads_with_return);
    return 0;
}
