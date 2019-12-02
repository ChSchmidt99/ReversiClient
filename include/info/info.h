#ifndef INFO_H
#define INFO_H

typedef struct _Player {
    int id;
    char* name;
} Player;

typedef struct _PlayerInfo {
    int count;
    Player* players;
} PlayerInfo;

#endif
