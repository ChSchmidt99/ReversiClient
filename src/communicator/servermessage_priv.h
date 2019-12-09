#include "communicator/servermessage.h"

ServerMessage* initServerMessage(char* messageReference, char* clearText, ServerMessageType type);
int isError(char* message);
ServerMessageType getType(const char* message);
char* getClearText(const char* message);