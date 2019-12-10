#include "communicator_priv.h"

//TODO: Parse Server Messages and wrap responses in ServerMessageType
//TODO: Cache if wrong response type
char* receiveServerGreeting(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    
    char* out = copyStringToNewMemoryAddr(message->messageReference + 2); 
    freeServerMessage(message);
    return out;
}

int hasAcceptedVersion(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    int result = 1;
    if (message->type == Error)
        result = 0;
    freeServerMessage(message);
    return 1;
}

GameKind receiveGameKind(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    GameKind out = parseGameKind(message->messageReference);
    freeServerMessage(message);
    return out;
}

GameKind parseGameKind(char* message){
    if (strcmp(message,REVERSI_GAMEKIND) == 0){
        return gamekind_Reversi;
    } else {
        return gamekind_Unkown;
    }
}

char* receiveGameName(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    char* out = copyStringToNewMemoryAddr(message->messageReference + 2);
    freeServerMessage(message);
    return out;
}

PlayerMeta* receivePlayerMeta(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    PlayerMeta* out = parsePlayerMeta(message->messageReference);
    freeServerMessage(message);
    return out;
}

PlayerMeta* parsePlayerMeta(char* message){
    size_t length = 0;
    char** tokens = slice(message," ",&length);
    
    //TODO: find better way to do this!
    if (length < 4)
        panic("unexpected playermeta string, expected 4 tokens");

    int number = atoi(tokens[2]);
    char* name = getNameFromPlayerMetaTokens(tokens,length);
    PlayerMeta* out = newPlayerMeta(number,name,1);
    freeTokens(tokens);
    return out;
}

char* getNameFromPlayerMetaTokens(char** tokens, size_t tokenCount){
    return joinTokens(&tokens[3],tokenCount - 3, " ");
}

PlayerMeta* receiveOtherPlayer(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    PlayerMeta* out = parseOtherPlayerMeta(message->messageReference);
    freeServerMessage(message);
    return out;
}

PlayerMeta* parseOtherPlayerMeta(char* message){
    size_t length = 0;
    char** tokens = slice(message," ",&length);
    
    //TODO: find better way to do this!
    if (length < 4)
        panic("unexpected playermeta string, expected 4 tokens");

    int number = atoi(tokens[1]);
    char* name = getNameForOtherPlayersTokens(tokens,length);
    int isReady = atoi(tokens[length - 1]);
    PlayerMeta* out = newPlayerMeta(number,name, isReady);
    freeTokens(tokens);
    return out;
}

char* getNameForOtherPlayersTokens(char** tokens, size_t tokenCount){
    size_t nameTokenCount = tokenCount - 3;
    char* nameTokens[nameTokenCount]; 
    size_t index = 0;
    for (size_t i = 2; i < tokenCount - 1; i++){
        nameTokens[index] = tokens[i];
        index++;
    }
    return joinTokens(nameTokens,nameTokenCount," ");
}

int reveiveTotalPlayers(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        return -1;

    char ASCInum = message->messageReference[8];
    return ASCInum - '0';
}

int nextMessageIsEndplayers(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    int result = 0;
    if (message->type == Error)
        panic(message->messageReference);
    else if (strcmp(message->messageReference, ENDPLAYERS_COMMAND) == 0)
        result = 1;

    freeServerMessage(message);
    return result;
}

//TODO: Cleanup
int waitForFirstMove(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);

    printf("Received: %s\n",message->messageReference);

    int moveTime = -1;
    if(message->type == Error){
        panic(message->messageReference);
    } else if(message->type == Quit){
        
    } else if (message->type == Wait){
        sendOkWait(connection);
        freeServerMessage(message);
        return waitForFirstMove(connection);
    } else if (message->type == Move){
        moveTime = parseMoveTime(message);
    } else {
        panic("Unexpected Command received");
    }
    freeServerMessage(message);
    return moveTime;
}

void receiveBoardDimensions(Connection* connection, size_t *rows, size_t *cols){
    ServerMessage* message = receiveServerMessage(connection);
    if(message->type == Error)
        panic(message->messageReference);
    
    size_t fieldDimensionTokenCount = 0;
    char** fieldDimensionTokens = slice(message->messageReference + 2," ",&fieldDimensionTokenCount);
    size_t dimensionCount = 0;
    char** dimensions = slice(fieldDimensionTokens[1],",",&dimensionCount);
    *cols = atoi(dimensions[0]);
    *rows = atoi(dimensions[1]);
    freeServerMessage(message);
    freeTokens(fieldDimensionTokens);
    freeTokens(dimensions);
}

//Use proper array instead of double ptr
char** receiveBoard(Connection* connection, size_t rows){
    logMessage("Receiving Board...\n",1);
    char** board = malloc(sizeof(char*) * rows);
    for(size_t i = 0; i < rows; i++){
        //TODO: Replace receiveServerMessage with safelyReceiveServerMessage, which unwraps Error
        ServerMessage* message = receiveServerMessage(connection);
        if(message->type == Error)
            panic(message->messageReference);
            
        board[i] = copyStringToNewMemoryAddr(message->messageReference + 2);
        freeServerMessage(message);
    }

    ServerMessage* message = receiveServerMessage(connection);
    if(message->type == Error)
        panic(message->messageReference);
    else if(message->type != Endfield)
        panic("Expected Endfield!");

    freeServerMessage(message);
    return board;
}

int receiveOkThink(Connection* connection){
    printf("Received OkThink\n");
    ServerMessage* message = receiveServerMessage(connection);
    int result = 0;
    if (message->type == OkThink)
        result = 1;
        
    freeServerMessage(message);
    return result;
}

void sendOkWait(Connection* connection){
    printf("Sending: '%s'\n",OK_WAIT_COMMAND);
    writeLineToServer(connection, OK_WAIT_COMMAND);
}

void sendThinking(Connection* connection){
    printf("Sending: '%s'\n",THINKING_COMMAND);
    writeLineToServer(connection, THINKING_COMMAND);
}

void sendMove(Connection* connection, char* move){
    printf("Sending Move: %s\n",move);
    char* moveString = concatStringToNewMemoryAddr(PLAY_COMMAND,move," ");
    writeLineToServer(connection,moveString);
    free(moveString);
}

void sendClientVersion(Connection* connection, const char* version){
    char* message = concatStringToNewMemoryAddr(VERSION,version," ");
    printf("Sending: '%s'\n",message);
    writeLineToServer(connection, message);
    free(message);
}

void sendGameId(Connection* connection, const char* gameID){
    char* message = concatStringToNewMemoryAddr(GAMEID,gameID," ");
    printf("Sending: '%s'\n",message);
    writeLineToServer(connection, message);
    free(message);
}

void sendPlayerPreference(Connection* connection, const char* preference){
    if (preference == NULL){
        printf("Sending: '%s'\n",PLAYERPREFERENCE);
        writeLineToServer(connection, PLAYERPREFERENCE);
    } else {
        char* message = concatStringToNewMemoryAddr(PLAYERPREFERENCE,preference," ");
        printf("Sending: '%s'\n",message);
        writeLineToServer(connection, message);
        free(message);
    }
}
