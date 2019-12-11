#include "minunit.h"
#include "utilities.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signalHandler(int sig){
    printf("Received Signal: %i\n",sig);
}

int childIsActive(pid_t pid){
    int childIsActive = waitpid(pid,NULL,WNOHANG);
    if (childIsActive == -1){
        panic("waitpid failed!");
        return 0;
    } else if (childIsActive == 0){
        return 0;
    } else {
        return 1;
    }
}

static char* testSignal(){
    pid_t processID = fork();
    if (processID < 0){
        panic("Failed to fork");
    } else if (processID == 0){
        //Child
        printf("sleeping...\n");
        sleep(3);
        printf("Finished sleeping, sending sig\n");
        kill(getppid(),SIGUSR1);
        printf("sleeping...\n");
        sleep(3);
        printf("Finished sleeping, sending sig\n");
        kill(getppid(),SIGUSR1);
        printf("sleeping...\n");
        sleep(3);
        printf("Finished sleeping, sending sig\n");
        kill(getppid(),SIGUSR1);
        printf("child returning\n");
    } else {
        //Parent
        struct sigaction newSig;
        newSig.sa_handler = &signalHandler;
        newSig.sa_flags = SA_RESTART;
        sigfillset(&newSig.sa_mask);
        if (sigaction(SIGUSR1,&newSig,NULL) == -1)
            panic("Failed to add sigaction\n");

        while (waitpid(processID,NULL,0) == 0);
        printf("parent returning\n");
    }
    return 0;
}

static char * playground() {
    //mu_run_test(testSignal);
    return 0;
}
 