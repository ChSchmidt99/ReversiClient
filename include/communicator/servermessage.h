#ifndef SERVERMESSAGE_H
#define SERVERMESSAGE_H

typedef enum _ServerMessageType {
    Error = 0,
    Prolog = 1,
    Wait = 2,
    Move = 3,
    Gameover = 4,
    MoveOk = 5
} ServerMessageType;

typedef struct _ServerMessage {
    char* clearText;
    char* messageReference;
    ServerMessageType type;
} ServerMessage;

ServerMessage* parseServerMessage(char* message);
void freeServerMessage(ServerMessage* serverMessage);
#endif