#include<stdlib.h>

typedef struct List_t List;

List* newEmptyList();
List* newListFromArray(void** data, size_t length);
void append(List* list, void* data);
void freeList(List* list);
void popLast(List* list);
void removeListItem(List* list, void* data);
void* getAtIndex(List* list, int i);
void iterateOverList(List* list, void(*handler)(void* data));
size_t getLength(List* list);