#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ai/mcst_tree.h"
#include "ai/mcst_boardstate.h"

static char* test_Descend(){
    char (*startingBoard)[BOARD_SIZE] = NewEmptyBoard();
    InitBoardWithStartingPosition(startingBoard);
    Node_mcst* rootNode = NewMCSTNode(startingBoard,MCST_PLAYER_1_SYMBOL,NULL);
    DescendAI(rootNode, MCST_PLAYER_1_SYMBOL);
    mu_assert("Should have 4 children", rootNode->childCount == 4);
    for (size_t i = 0; i < rootNode->childCount; i++)
        mu_assert("All Children should have 1 game", rootNode->childNodes[i]->gameCount == 1);
    FreeMCSTTree(rootNode);
    return 0;
}

static char * run_tree_tests() {
    mu_run_test(test_Descend);
    return 0;
}
