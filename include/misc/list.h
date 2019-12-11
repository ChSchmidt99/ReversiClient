#ifndef LIST_H
#define LIST_H

    #include<stdlib.h>

    typedef struct List_t List;

    List* newEmptyList();
    List* newListFromArray(void** data, size_t length);
    void append(List* list, void* data);
    // Appends the List stored in src to the List in dest
    void appendList(List* dest,List* src);
    void freeList(List* list);
    void freeListContents(List* list);
    void popLast(List* list);
    void removeListItem(List* list, void* data);
    void* getAtIndex(List* list, size_t i);
    void iterateOverList(List* list, void(*handler)(void* data));
    size_t getLength(List* list);

#endif