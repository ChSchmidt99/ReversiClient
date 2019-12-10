#include <stdio.h>
#include <stdlib.h>

#include "communicator/prolog.h"
#include "communicator/servermessage.h"
#include "utilities.h"

void printAndFree(char* message);
void printPlayerMeta(PlayerMeta* meta);
GameInstance* initGameInstance(PlayerMeta* ownPlayer, GameKind gameKind, size_t opponentCount, PlayerMeta* opponents[opponentCount],size_t boardSize, char* gameName);

//TODO: Splitup and clean function
GameInstance* initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference){

    char* message = receiveServerGreeting(connection);
    printAndFree(message);

    sendClientVersion(connection, version);

    if (!hasAcceptedVersion(connection))
        panic("Server Did not accept Client Version");
    else 
        printf("Server Accepted Client Version\n");
    
    sendGameId(connection, gameId);

    GameKind gameKind = receiveGameKind(connection);

    char* gameName = receiveGameName(connection);

    sendPlayerPreference(connection, playerPreference);

    PlayerMeta* ownMeta = receivePlayerMeta(connection);

    int totalPlayers = reveiveTotalPlayers(connection);
    PlayerMeta* opponents[totalPlayers];
    for (int i = 0; i < totalPlayers - 1; i++){
        PlayerMeta* otherPlayer = receiveOtherPlayer(connection);
        opponents[i] = otherPlayer;
    }

    if (!nextMessageIsEndplayers(connection))
        panic("Expected ENDPLAYERS");


    int moveTime = waitForFirstMove(connection);
    if (moveTime == -1)
        panic("Failed to wait for first Move");

    size_t rows = 0;
    size_t cols = 0;
    receiveBoardDimensions(connection,&rows,&cols);

    return initGameInstance(ownMeta,gameKind,totalPlayers - 1,opponents, rows, gameName);
}

GameInstance* initGameInstance(PlayerMeta* ownPlayer, GameKind gameKind, size_t opponentCount, PlayerMeta* opponents[opponentCount],size_t boardSize, char* gameName){
    GameInstance* newInstance = malloc(sizeof(GameInstance) + sizeof(PlayerMeta*) * opponentCount);
    newInstance->gameKind = gameKind;
    newInstance->ownPlayer = ownPlayer;
    newInstance->opponentCount = opponentCount;
    newInstance->gameName = gameName;
    newInstance->boardSize = boardSize;
    for (size_t i = 0; i < opponentCount; i++){
        newInstance->opponents[i] = opponents[i];
    }
    return newInstance;
}

void freeGameInstance(GameInstance* instance){
    //TODO: Check if pointers are freed sufficientally
    /*
    for (size_t i = 0; i < instance->opponentCount; i++){
        freePlayerMeta(instance->opponents[i]);
    }
    */
    free(instance->gameName);
    free(instance->ownPlayer);
    free(instance);
}

void printGameInstanceDetails(GameInstance* gameInstance){
    printf("Game Name: %s\n",gameInstance->gameName);
    char* gameKindStr = gameKindString(gameInstance->gameKind);
    printf("Game Kind: %s\n",gameKindStr);
    free(gameKindStr);
    printf("Player Info: \n");
    printPlayerMeta(gameInstance->ownPlayer);
    printf("Number of Opponents: %zu\n",gameInstance->opponentCount);
    printf("Opponents Info: \n");
    for (size_t i = 0; i < gameInstance->opponentCount; i++)
        printPlayerMeta(gameInstance->opponents[i]);

    printf("BoardSize: %zu\n",gameInstance->boardSize);
}

void printPlayerMeta(PlayerMeta* meta){
    printf("Player '%s' has number: %i ",meta->name, meta->number);
    if (meta->isReady)
        printf("Ready\n");
    else
        printf("Not Ready\n");
}

char* gameKindString(GameKind gameKind){
    if (gameKind == gamekind_Reversi)
        return copyStringToNewMemoryAddr("Reversi");
    else 
        return copyStringToNewMemoryAddr("Unknown");
}


void printAndFree(char* message){
    printf("%s\n",message);
    free(message);
}