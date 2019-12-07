#include "communicator/servermessage.h"

int isError(char* message);
ServerMessageType getType(char* message);
char* getClearText(char* message);
void* getData(char* message);