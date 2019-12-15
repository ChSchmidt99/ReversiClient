#ifndef SERVERMESSAGE_H
#define SERVERMESSAGE_H

typedef enum _ServerMessageType {
    Error = 0,
    Prolog = 1,
    Wait = 2,
    Move = 3,
    Gameover = 4,
    MoveOk = 5,
    Endfield = 6,
    Quit = 7,
    OkThink = 8,
    FieldCommand = 9,
    Player0Won = 10,
    Player1Won = 11
} ServerMessageType;

typedef struct _ServerMessage {
    char* messageReference;
    ServerMessageType type;
} ServerMessage;

ServerMessage* parseServerMessage(char* message);
void freeServerMessage(ServerMessage* serverMessage);
int parseMoveTime(ServerMessage* message);

#endif
