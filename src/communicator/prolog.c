#include <stdio.h>
#include <stdlib.h>

#include "communicator/prolog.h"
#include "communicator/servermessage.h"
#include "utilities.h"

void printAndFree(char* message);
void printPlayerMeta(PlayerMeta* meta);
GameInstance* initGameInstance(PlayerMeta* ownPlayer, GameKind gameKind, size_t opponentCount, PlayerMeta* opponents[opponentCount], char* gameName);

//TODO: Splitup and clean function
GameInstance* initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference){

    char* message = receiveServerGreeting(connection);
    if (message == (char*)-1)
        return (GameInstance*) -1;

    printAndFree(message);

    sendClientVersion(connection, version);

    if (!hasAcceptedVersion(connection))
        panic("Server Did not accept Client Version");
    else
        printf("Server Accepted Client Version\n");

    sendGameId(connection, gameId);

    GameKind gameKind = receiveGameKind(connection);
    if (gameKind == (GameKind) -1)
        return (GameInstance*) -1;

    char* gameName = receiveGameName(connection);
    if (gameName == (char*) -1)
        return (GameInstance*) -1;

    sendPlayerPreference(connection, playerPreference);

    PlayerMeta* ownMeta = receivePlayerMeta(connection);
    if (ownMeta == (PlayerMeta*) -1)
        return (GameInstance*) -1;

    int totalPlayers = reveiveTotalPlayers(connection);
    if (totalPlayers == -1)
        return (GameInstance*) -1;

    PlayerMeta* opponents[totalPlayers];
    for (int i = 0; i < totalPlayers - 1; i++){
        PlayerMeta* otherPlayer = receiveOtherPlayer(connection);
        if (otherPlayer == (PlayerMeta*) -1)
            return (GameInstance*) -1;
        opponents[i] = otherPlayer;
    }

    if (!nextMessageIsEndplayers(connection))
        return (GameInstance*) -1;

    return initGameInstance(ownMeta,gameKind,totalPlayers - 1,opponents,gameName);
}

GameInstance* initGameInstance(PlayerMeta* ownPlayer, GameKind gameKind, size_t opponentCount, PlayerMeta* opponents[opponentCount], char* gameName){
    GameInstance* newInstance = malloc(sizeof(GameInstance) + sizeof(PlayerMeta*) * opponentCount);
    newInstance->gameKind = gameKind;
    newInstance->ownPlayer = ownPlayer;
    newInstance->opponentCount = opponentCount;
    newInstance->gameName = gameName;
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
}

void printPlayerMeta(PlayerMeta* meta){
    printf("Player '%s' has number: %i ",meta->name, meta->number);
    if (meta->isReady)
        printf("Ready\n");
    else
        printf("Not Ready\n");
}

void printAndFree(char* message){
    printf("%s\n",message);
    free(message);
}