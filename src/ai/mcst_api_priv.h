#include "ai/mcst_api.h"
#include "ai/mcst_tree.h"

int getChildMaxGames(Node_mcst* parent);
Node_mcst* getChildWithMaxScore(Node_mcst* parent);
Node_mcst* getChildWithMaxScoreAndMaxGames(Node_mcst* parent, time_t latestReturnTimestamp, char forPlayer);
Node_mcst* buildMCSTTree(char(*board)[BOARD_SIZE], char forPlayer, time_t returnTimestamp, size_t threads);

typedef struct {
    Node_mcst* rootNode; 
    char forPlayer;
    time_t returnTimestamp;
} expandMCSTTreeInput;

expandMCSTTreeInput* newExpandMCSTTreeInput(Node_mcst* rootNode, char forPlayer, time_t returnTimestamp);
void freeExpandMCSTTreeInput(expandMCSTTreeInput* input);

void* expandMCSTTree(void* input);