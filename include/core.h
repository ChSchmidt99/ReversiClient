#ifndef CORE_H
#define CORE_H

    typedef enum _GameKind {
        gamekind_Unkown = 0,
        gamekind_Reversi = 1
    } GameKind;

    typedef struct _PlayerMeta {
        int number;
        char* name;
        int isReady;
    } PlayerMeta;

    typedef struct _InputParams {
        char* hostName;
        char* portNumber;
        char* gameKind;
        char* gameId;
        char* playerPreference;
    } InputParams;

    PlayerMeta* newPlayerMeta(int number, char* name, int isReady);
    void freePlayerMeta(PlayerMeta* meta);
    char* gameKindString(GameKind gameKind);
#endif