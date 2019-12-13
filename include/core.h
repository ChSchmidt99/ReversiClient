#ifndef CORE_H
#define CORE_H
    
    #include <sys/types.h>
    #define MAX_OPPONENTS 4
    #define CONNECTION_TIMEOUT_IN_SEC 60

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

    typedef struct _ProcessInfo {
        pid_t communicatorPID;
        pid_t thinkerPID;
        int fd[2];
    } ProcessInfo;

    PlayerMeta* newPlayerMeta(int number, char* name, int isReady);
    void freePlayerMeta(PlayerMeta* meta);

    ProcessInfo* newProcessInfo(int pipe[2], pid_t thinkerPID, pid_t communicatorPID);
    void freeProcessInfo(ProcessInfo* procInfo);

    char* gameKindString(GameKind gameKind);
#endif