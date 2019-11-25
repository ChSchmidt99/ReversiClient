#ifndef FIELD_H
#define FIELD_H

#include <stdbool.h>

typedef struct _Field {
    unsigned int width, height;
    char* columns;
} Field;

Field* createField(unsigned int width, unsigned int height);

void loadField(Field* field, char* str[]);

void loadFieldFromSHM(Field* field, const char* str);

void printField(Field* field);

char getColumn(Field* field, unsigned int x, unsigned int y);

char* toString(unsigned int x, unsigned int y);

void freeField(Field* field);

bool isValidMove(Field* field, char own, unsigned int x, unsigned int y);

#endif
