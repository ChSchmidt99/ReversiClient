#include "communicator/servermessage.h"


ServerMessage* initServerMessage(char* messageReference, ServerMessageType type);
int isError(const char* message);
ServerMessageType getType(const char* message);
