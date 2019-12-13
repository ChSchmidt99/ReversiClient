#include "misc/list.h"
#include "misc/utilities.h"

#include <stdio.h>

typedef struct Node_t Node;
struct Node_t{
    Node* nextEntry;
    void* data;
};

struct List_t{
    size_t length;
    Node* head;
};

List* newEmptyList(){
    List* newList = safeMalloc(sizeof(List));
    newList->length = 0;
    newList->head = NULL;
    return newList;
}

List* newListFromArray(void** data, size_t length){
    Node* previousList = NULL;
    for (int i = length - 1; i >= 0; i--){
        Node* list = safeMalloc(sizeof(Node));
        list->nextEntry = previousList;
        list->data = data[i];
        previousList = list;
    }
    List* newList = safeMalloc(sizeof(List));
    newList->length = length;
    newList->head = previousList;
    return newList;
}

void appendToNode(Node* node, void* data){
    if (node->nextEntry == NULL){
        Node* newList = safeMalloc(sizeof(Node));

        newList->nextEntry = NULL;
        newList->data = data;
        node->nextEntry = newList;
    } else {
        appendToNode(node->nextEntry, data);
    }
}

void append(List* list, void* data){
    if (list->head == NULL){
        Node* newNode = safeMalloc(sizeof(Node));
        newNode->nextEntry = NULL;
        newNode->data = data;
        list->head = newNode;
    } else {
        appendToNode(list->head, data);
    }
    list->length++;
}

void appendNodeToNode(Node* dest, Node* src){
    if (dest->nextEntry == NULL){
        dest->nextEntry = src;
    } else {
        appendNodeToNode(dest->nextEntry, src);
    }
}

void appendList(List* dest,List* src){
    if (dest->head == NULL){
        dest->head = src->head;
    } else {
        appendNodeToNode(dest->head, src->head);
    }
    dest->length += src->length;
    free(src);
}

void deleteNodes(Node* nodes){
    if (nodes->nextEntry != NULL){
        deleteNodes(nodes->nextEntry);
    }
    free(nodes);
}

void freeList(List* list){
    if (list->head != NULL) {
        deleteNodes(list->head);
    }
    free(list);
}

void popLastNode(Node* nodes){
    if (nodes->nextEntry == NULL) {
        deleteNodes(nodes);
    } else if (nodes->nextEntry->nextEntry == NULL){
        deleteNodes(nodes->nextEntry);
        nodes->nextEntry = NULL;
    } else {
        popLastNode(nodes->nextEntry);
    }
}

void popLast(List* list){
    if (list->head == NULL) 
        return;
    if (list->head != NULL)
        popLastNode(list->head);
    --list->length;
    if (list->length == 0)
        list->head = NULL;
}

void* getNodeAtIndex(Node* list, int i){
    if(i == 0){
        return list->data;
    } else if (list->data == NULL){
        return NULL;
    } else {
        return getNodeAtIndex(list->nextEntry, --i);
    }
}

void* getAtIndex(List* list, size_t i){
    if (i > list->length - 1 || list->length == 0){
        return NULL;
    } else {
        return getNodeAtIndex(list->head, i);
    }
}

void iterateOverNodes(Node* list, void(*handler)(void* data)){
    handler(list->data);
    if (list->nextEntry != NULL){
        iterateOverNodes(list->nextEntry, handler);
    }
}

void iterateOverList(List* list, void(*handler)(void* data)){
    if (list->head != NULL){
        iterateOverNodes(list->head,handler);
    }
}

void removeNode(Node* previouseNode, Node* nodeToRemove){
    previouseNode->nextEntry = nodeToRemove->nextEntry;
    free(nodeToRemove);
}

void removeItem(Node* previouseNode, Node* remainigList, void* data, int* count){
    if (remainigList->data == data){
        removeNode(previouseNode, remainigList);
        *count += 1;
    } 
    if (remainigList->nextEntry != NULL)
        removeItem(remainigList, remainigList->nextEntry, data, count);
}

void removeListItem(List* list, void* data){
    int count = 0;
    if (list->head != NULL){
        if (list->head->data == data){
            Node* removedItem = list->head;
            list->head = removedItem->nextEntry;
            free(removedItem);
            ++count;
        } else {
            removeItem(list->head,list->head->nextEntry,data, &count);   
        }
    }
    list->length -= count;
}

size_t getLength(List* list){
    return list->length;
}

void FreeNodeContents(Node* nodes){
    free(nodes->data);
    if (nodes->nextEntry != NULL)
        FreeNodeContents(nodes);
}

void freeListContents(List* list){
    FreeNodeContents(list->head);
}