
typedef enum _ServerMessageStage {
    Error = 0,
    Prolog = 1,
    Spielverlauf = 2,
    Spielzug = 3
} ServerMessageType;

typedef enum _ServerMessageCommand {
    Unknown = 0,
    AcceptingConnections = 1,
    VersionAccepted = 2,
    PlayingGameKind = 3,
    GameName = 4,
    PlayerMeta = 5,
    PlayerCount = 6,
    OtherPlayer = 7
} ServerMessageCommand;

typedef struct _ServerMessage {
    ServerMessageType type;
    ServerMessageCommand command;
    char* message;
    void* data;
} ServerMessage;

void freeServerMessage(ServerMessage* serverMessage);
void printServerMessage(ServerMessage* serverMessage);

ServerMessage* parseServerVersion(char* data);