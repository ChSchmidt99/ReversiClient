#include "optionreader.h"
#include "config.h"
#include "communicator/connection.h"
#include "communicator/prolog.h"
#include "thinker/thinker.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//TODO: wait still needed? not available on macOS
//#include <wait.h>

#define VERSION_NUMBER "2.3"
#define DEFAULT_CONFIG_PATH "./client.conf"

int main(int argc, char *argv[]) {
    char* gameId = readGameID(argc,argv);
    if (gameId == NULL) {
        printf("GameId must be set!\n");
        return EXIT_FAILURE;
    }
    char* playerPreference = readPreferencedPlayerNumber(argc,argv);

    pid_t thinker;
    int fd[2];

    if(pipe(fd) < 0) {
        perror("Could not create pipe");
        exit (EXIT_FAILURE);
    }
    if((thinker = fork()) < 0) {
        perror("Could not fork for thinker process");
        exit(EXIT_FAILURE);
    }

    if(thinker > 0) {
        printf("[PARENT/%i] Started connector child\n", thinker);

        // start the thinker process
        printf("[PARENT] Starting thinker..\n");
        tick(NULL, fd);

        if((waitpid (thinker, NULL, 0)) < 0) {
            perror ("Error waiting for child processes to die");
            exit (EXIT_FAILURE);
        }

        printf("Connector died, exiting..\n");
    } else {
        printf("[CHILD/CONNECTOR] Starting connector..\n");
        // start the connector process
        char* configFilePath = readConfigFilePath(argc, argv);
        if (configFilePath == NULL) {
            configFilePath = DEFAULT_CONFIG_PATH;
        }
        Params* params = getParamsFromFile(configFilePath);
        Connection* connection = newConnection(params->hostName,params->portNumber);
        freeParams(params);

        connectToServer(connection);
        initiateProlog(connection,VERSION_NUMBER,gameId, playerPreference);
        disconnectFromServer(connection);
        freeConnection(connection);
        free(gameId);
        free(playerPreference);

        printf("[CONNECTOR] Finished prolog, exiting..\n");
        kill(thinker, SIGUSR1);
        return EXIT_SUCCESS;
    }
}