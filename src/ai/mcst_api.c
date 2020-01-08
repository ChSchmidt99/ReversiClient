#include "mcst_api_priv.h"
#include "ai/mcst_boardstate.h"
#include "misc/utilities.h"
#include "threadmanager/threadmanager.h"

#include <pthread.h>
#include <stdio.h>

#define BOARDSIZE 8

char* CalculateNextMoveAI(char(*board)[BOARD_SIZE], char forPlayer, time_t minCalculatingTime, time_t maxCalculatingTime, size_t threads){
    time_t soonestReturnTimestamp = time(NULL) + minCalculatingTime;
    time_t latestReturnTimestamp = time(NULL) + maxCalculatingTime;
    Node_mcst* mcstTree = buildMCSTTree(board,forPlayer,soonestReturnTimestamp,threads);
    Node_mcst* chosenChild = getChildWithMaxScoreAndMaxGames(mcstTree, latestReturnTimestamp, forPlayer);    
    char* move = GetMoveForBoardStates(board,chosenChild->boardState);
    printf("Tree For Player %c, number of Games: %i\n",forPlayer,mcstTree->gameCount);
    //IMPROVEMENT: Maybe free Tree after return for performance increase
    FreeMCSTTree(mcstTree);
    return move;
}

//TODO: Cleanup
char* CalculateNextMoveAIOptimizedThreads(char(*board)[BOARD_SIZE], char forPlayer, long long calcTimeinMs){
    long long returnTimestamp = currentTimestampInMs() + calcTimeinMs;
    
    Node_mcst* rootNode = NewMCSTNode(board,forPlayer,NULL);
    size_t childCount = 0;
    Node_mcst** children = CalculateAllChildren(rootNode,&childCount);

    pthread_t threadIDs[childCount];
    expandMCSTTreeInput* input[childCount];

    for (size_t i = 0; i < childCount; i++){
        input[i] = newExpandMCSTTreeInput(children[i],forPlayer,returnTimestamp);
        if (startThread(&threadIDs[i],expandMCSTTree,input[i]) != 0)
            panic("Failed to start Thread");
    }

    joinThreads(childCount,threadIDs,NULL);

    rootNode->childCount = childCount;
    rootNode->childNodes = children;

    for (size_t i = 0; i < childCount; i++){
        //TODO: Split up
        freeExpandMCSTTreeInput(input[i]);
        rootNode->gameCount += children[i]->gameCount;
        rootNode->winCount += children[i]->winCount;
    }
    
    Node_mcst* chosenChild = getChildWithMaxScore(rootNode);
    char* move = GetMoveForBoardStates(board,chosenChild->boardState);
    
    printf("Tree For Player %c, number of Games: %i\n",forPlayer,rootNode->gameCount);
    
    //TODO: Maybe free Tree after return for performance increase

    for (size_t i = 0; i < childCount; i++){
        FreeMCSTTree(children[i]);
    }
    return move;
}

Node_mcst* buildMCSTTree(char(*board)[BOARD_SIZE], char forPlayer, time_t returnTimestamp, size_t threads){
    char (*inputBoard)[BOARD_SIZE] = CopyBoard(board);
    Node_mcst* rootNode = NewMCSTNode(inputBoard,forPlayer,NULL);
    expandMCSTTreeInput* input = newExpandMCSTTreeInput(rootNode,forPlayer,returnTimestamp);
    pthread_t threadIds[threads];
    if (startMultipleThreads(threads,threadIds,&expandMCSTTree,input) != 0)
        panic("Failed to start Threads");
    if (joinThreads(threads,threadIds,NULL) != 0)
        panic("Failed to join Threads");
    freeExpandMCSTTreeInput(input);
    return rootNode;
}

expandMCSTTreeInput* newExpandMCSTTreeInput(Node_mcst* rootNode, char forPlayer, long long returnTimestamp){
    expandMCSTTreeInput* out = safeMalloc(sizeof(expandMCSTTreeInput));
    out->rootNode = rootNode;
    out->forPlayer = forPlayer;
    out->returnTimestamp = returnTimestamp;
    return out;
}

void freeExpandMCSTTreeInput(expandMCSTTreeInput* input){
    free(input);
}

void* expandMCSTTree(void* input){
    expandMCSTTreeInput* in = input;
    while (currentTimestampInMs() < in->returnTimestamp){
        DescendAI(in->rootNode, in->forPlayer);
    } 
    return NULL;
}

Node_mcst* getChildWithMaxScoreAndMaxGames(Node_mcst* parent, time_t latestReturnTimestamp, char forPlayer){
    //IMPROVEMENT: Only descend on the node with the highest score?
    while (time(NULL) < latestReturnTimestamp){
        int maxGames = getChildMaxGames(parent);
        Node_mcst* maxScore = getChildWithMaxScore(parent);
        if (maxScore->gameCount == maxGames)
            return maxScore;
        else 
            DescendAI(parent, forPlayer);
    }
    return getChildWithMaxScore(parent);
}


Node_mcst* getChildWithMaxScore(Node_mcst* parent){
    if (parent->childCount == 0)
        panic("not able to get child with max score, parent has 0 children");

    double maxScore = 0;
    Node_mcst* maxScoreNode = parent->childNodes[0];
    for (size_t i = 0; i < parent->childCount; i++){
        double score = (double)parent->childNodes[i]->winCount / (double)parent->childNodes[i]->gameCount;
        if (score > maxScore){
            maxScore = score;
            maxScoreNode = parent->childNodes[i];
        }
    }
    return maxScoreNode;
}

int getChildMaxGames(Node_mcst* parent){
    int maxGames = 0;
    for (size_t i = 0; i < parent->childCount; i++){
        if (parent->childNodes[i]->gameCount > maxGames){
            maxGames = parent->childNodes[i]->gameCount;
        }
    }
    return maxGames;
}