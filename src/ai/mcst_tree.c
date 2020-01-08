#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "misc/utilities.h"
#include "mcst_tree_priv.h"
#include "ai/mcst_boardstate.h"
#include "misc/list.h"

Node_mcst* NewMCSTNode(char(*board)[BOARD_SIZE],char playerForNextTurn, Node_mcst* previousNode){
    Node_mcst* node = safeMalloc(sizeof(Node_mcst));
    node->boardState = board;
    node->playerForNextTurn = playerForNextTurn;
    node->parentNode = previousNode;
    node->childNodes = NULL;
    node->childCount = 0;
    node->gameCount = 0;
    node->winCount = 0;
    return node;
}

void FreeMCSTTree(Node_mcst* rootNode){
    for (size_t i = 0; i < rootNode->childCount; i++)
        FreeMCSTTree(rootNode->childNodes[i]);
    freeMCSTNode(rootNode);
}

void PrintNode(Node_mcst* node){
    if (node == NULL)
        printf("Node is NULL");
    else {
        printf("Node childCount: %zu\n",node->childCount);
        printf("Node gameCount: %i\n",node->gameCount);
        printf("Node winCount: %i\n",node->winCount);
        printf("Node Board State:\n");
        PrintBoard(node->boardState);
    }
}

void freeMCSTNode(Node_mcst* node){
    FreeBoard(node->boardState);
    free(node);
}

void DescendAI(Node_mcst* rootNode, char expectedWinner){
    if (isLeafNode(rootNode)){
        if (!GameIsFinished(rootNode->boardState))
            expand(rootNode,expectedWinner);        
    } else {
        Node_mcst* nextNode = selectNextNodeUTC(rootNode);
        DescendAI(nextNode,expectedWinner);
    }
}

/*
void expand(Node_mcst* leafNode, char expectedWinner){
    addAllChildren(leafNode);
    simulateAllChildren(leafNode, expectedWinner);
    backpropagateAllChildren(leafNode);
}
*/

void expand(Node_mcst* leafNode, char expectedWinner){
    size_t childCount = 0;
    Node_mcst** nodes = CalculateAllChildren(leafNode,&childCount);
    simulateAllNodes(nodes,childCount,expectedWinner);
    mergeChildren(nodes, childCount,leafNode);
}

void mergeChildren(Node_mcst** children, size_t childCount, Node_mcst* parent){
    //TODO: Centralize backprop, dont use winCount
    if (parent->childCount == 0){
        parent->childNodes = children;
        parent->childCount = childCount;
        backpropagateAllChildren(parent);
    } else {
        for (size_t i = 0; i < parent->childCount; i++){
            parent->childNodes[i]->gameCount += children[i]->gameCount;
            parent->childNodes[i]->winCount += children[i]->winCount;
            backpropagate(parent,children[i]->winCount);
            freeMCSTNode(children[i]);
        }
    }
}

//IMPROVEMENT: Instead of simulating use neural network
void simulateAllNodes(Node_mcst** nodes, size_t nodeCount,char expectedWinner){
    for (size_t i = 0; i < nodeCount; i++){
        char winner = rolloutForNode(nodes[i]);
        nodes[i]->gameCount++;
        if (expectedWinner == winner)
            nodes[i]->winCount++;
    }
}

void simulateAllChildren(Node_mcst* parentNode, char expectedWinner){
    for (size_t i = 0; i < parentNode->childCount; i++){
        char winner = rolloutForNode(parentNode->childNodes[i]);
        parentNode->childNodes[i]->gameCount++;
        if (expectedWinner == winner)
            parentNode->childNodes[i]->winCount++;
    }
}

char rolloutForNode(Node_mcst* parentNode){
    char (*finishedBoard)[BOARD_SIZE] = simulateGame(CopyBoard(parentNode->boardState),parentNode->playerForNextTurn);
    char winner = GetWinner(finishedBoard);
    FreeBoard(finishedBoard);
    return winner;
}

char (*simulateGame(char (*startingBoard)[BOARD_SIZE],char currentPlayer))[BOARD_SIZE]{
    if (GameIsFinished(startingBoard))
        return startingBoard;

    char (*nextBoard)[BOARD_SIZE] = GetRandomPossibleBoardState(startingBoard,currentPlayer);
    FreeBoard(startingBoard);
    return simulateGame(nextBoard,GetNextPlayer(nextBoard,currentPlayer));
}

void backpropagateAllChildren(Node_mcst* parentNode){
    for (size_t i = 0; i < parentNode->childCount; i++){
        backpropagate(parentNode,parentNode->childNodes[i]->winCount);
    }
}

void backpropagate(Node_mcst* fromNode, int win){
    if (fromNode != NULL){
        fromNode->gameCount++;
        fromNode->winCount += win;
        backpropagate(fromNode->parentNode,win);
    }
}

Node_mcst* selectNextNodeUTC(Node_mcst* fromNode){
    double maxUTC = -1;
    Node_mcst* out = NULL;
    for (size_t i = 0; i < fromNode->childCount; i++){
        double utc = calculateUTC(fromNode->childNodes[i],EXPLORATION_FACTOR, fromNode->gameCount);
        if (utc > maxUTC){
            out = fromNode->childNodes[i];
            maxUTC = utc;
        }
    }
    return out;
}

double calculateUTC(Node_mcst* node, double explorationFactor, int parentTotalGames){
    return node->winCount / node->gameCount + explorationFactor * sqrt(log(parentTotalGames)/node->gameCount);
}

Node_mcst** CalculateAllChildren(Node_mcst* parentNode, size_t* childCountOut){
    List* states = GetPossibleBoardStates(parentNode->boardState, parentNode->playerForNextTurn);
    size_t numberOfStates = getLength(states);
    if (numberOfStates > 0){
        Node_mcst** nodes = safeMalloc(sizeof(Node_mcst*) * numberOfStates);
        for (size_t i = 0; i < numberOfStates; i++){
            char (*state)[BOARD_SIZE] = getAtIndex(states,i);
            Node_mcst* newNode = NewMCSTNode(state, GetNextPlayer(state,parentNode->playerForNextTurn), parentNode);
            nodes[i] = newNode;
        }
        freeList(states);
        *childCountOut = numberOfStates;
        return nodes;
    } else {
        freeList(states);
        *childCountOut = 0;
        return NULL;
    }
    
}

void addAllChildren(Node_mcst* parentNode){
    List* states = GetPossibleBoardStates(parentNode->boardState, parentNode->playerForNextTurn);
    size_t numberOfStates = getLength(states);
    if (numberOfStates > 0){
        Node_mcst** nodes = safeMalloc(sizeof(Node_mcst*) * numberOfStates);
        for (size_t i = 0; i < numberOfStates; i++){
            char (*state)[BOARD_SIZE] = getAtIndex(states,i);
            Node_mcst* newNode = NewMCSTNode(state, GetNextPlayer(state,parentNode->playerForNextTurn), parentNode);
            nodes[i] = newNode;
        }
        parentNode->childCount = numberOfStates;
        parentNode->childNodes = nodes;
    }
    freeList(states);
}

bool isLeafNode(Node_mcst* node){
    if (node->childCount == 0)
        return true;
    else 
        return false;
}