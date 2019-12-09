#ifndef PROLOG_H
#define PROLOG_H
    #include "communicator/communicator.h"
    #include "info/info.h"

    typedef struct _GameInstance {
        PlayerMeta* ownPlayer;
        GameKind gameKind;
        size_t opponentCount;
        char* gameName;
        PlayerMeta* opponents[];
    } GameInstance;
    
    GameInstance* initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference);
    void printGameInstanceDetails(GameInstance* gameInstance);
    char* gameKindString(GameKind gameKind);
    void freeGameInstance(GameInstance* instance);
#endif