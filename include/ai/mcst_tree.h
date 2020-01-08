#ifndef MCST_TREE_H
#define MCST_TREE_H

    #include "mcst_boardstate.h"
    #include <stdlib.h>
    #define EXPLORATION_FACTOR sqrt(2)

    typedef struct _Node_mcst Node_mcst;

    //IMPROVEMENT: Use Binary representation for game board
    struct _Node_mcst{
        char(*boardState)[BOARD_SIZE];
        char playerForNextTurn;
        int gameCount;
        int winCount;
        Node_mcst* parentNode;
        Node_mcst** childNodes;
        size_t childCount;
    };

    Node_mcst* NewMCSTNode(char(*board)[BOARD_SIZE],char playerForNextTurn, Node_mcst* previousNode);
    void FreeMCSTTree(Node_mcst* rootNode);    

    void DescendAI(Node_mcst* rootNode, char expectedWinner);
    void PrintNode(Node_mcst* node);
    Node_mcst** CalculateAllChildren(Node_mcst* parentNode, size_t* childCountOut);
#endif