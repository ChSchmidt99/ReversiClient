#include "ai/mcst_tree.h"

bool isLeafNode(Node_mcst* node);
void addAllChildren(Node_mcst* parentNode);
Node_mcst* selectNextNodeUTC(Node_mcst* fromNode);
void simulateAllChildren(Node_mcst* parentNode, char expectedWinner);
void backpropagateAllChildren(Node_mcst* parentNode);
void expand(Node_mcst* leafNode, char expectedWinner);
double calculateUTC(Node_mcst* node, double explorationFactor, int parentTotalGames);
char rolloutForNode(Node_mcst* parentNode);
void backpropagate(Node_mcst* fromNode, int win);
void freeMCSTNode(Node_mcst* node);
char (*simulateGame(char (*startingBoard)[BOARD_SIZE],char currentPlayer))[BOARD_SIZE];


void simulateAllNodes(Node_mcst** nodes, size_t nodeCount,char expectedWinner);
void mergeChildren(Node_mcst** children, size_t childCount, Node_mcst* parent);