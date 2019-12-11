#include "misc/minunit.h"
#include "misc/utilities.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signalHandler(int sig){
    printf("Received Signal: %i\n",sig);
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
        exit(0);
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

int pipeIsReady(int fd){
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd,&rfds);
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    int retvel = select(fd+1,&rfds, NULL,NULL,&timeout);
    printf("Gor Retvel!: %i\n",retvel);
    if (retvel == -1){    
        perror("Failed to select");
        return 0;
    } else {
        return retvel;
    }
}

static char* test_select(){
    int fd[2];
    if (pipe(fd) < 0)
        panic("Failed to pipe");

    pid_t processID = fork();
    if (processID < 0){
        panic("Failed to fork");
    } else if (processID == 0){
        //Child
        //close write
        close(fd[1]);
        if (pipeIsReady(fd[0])){
            printf("Pipe Ready!\n");
            char buff[10];
            read(fd[0],buff,10);
            printf("From Pipe: %s\n",buff);
        } else {
            printf("Timeout!\n");
        }
        exit(0);
    } else {
        //Parent
        //close read
        close(fd[0]);

        sleep(1);

        char* in = "Hallo";
        if (write(fd[1],in,10) == -1)
            panic("Failed to write");
        
        printf("Wrode to Pipe\n");

        while (waitpid(processID,NULL,0) == 0);
    }
    return 0;
}


static char * playground() {
    //mu_run_test(testSignal);
    //mu_run_test(test_select);
    return 0;
}
 