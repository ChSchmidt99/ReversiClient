#ifndef PROLOG_H
#define PROLOG_H
    #include "communicator/communicator.h"

    //IMPROVEMENT: Use rows and cols instead of boardSize
    typedef struct _GameInstance {
        PlayerMeta* ownPlayer;
        GameKind gameKind;
        char* gameName;
        size_t opponentCount;
        PlayerMeta* opponents[];
    } GameInstance;
    
    GameInstance* initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference);
    void printGameInstanceDetails(GameInstance* gameInstance);
    void freeGameInstance(GameInstance* instance);
#endif