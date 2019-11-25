#include "thinker/field.h"
#include <stdlib.h>
#include <stdio.h>

Field* createField(unsigned int width, unsigned int height) {
    Field *field = malloc(sizeof(Field));
    field->width = width;
    field->height = height;
    field->columns = malloc(sizeof(char) * width * height);
    return field;
}

char* toString(unsigned int x, unsigned int y) {
    char j = "ABCDEFGH"[x];
    char i = "12345678"[y];
    char* result = malloc(sizeof(char) * 2);
    result[0] = j;
    result[1] = i;
    return result;
}

void loadField(Field* field, char* str[]) {
    for(unsigned int row = 0; row < field->width ; row++) {
        char* rowStr = str[row];
        for(unsigned int column = 0 ; column < field->height; column++) {
            unsigned int index = row * field->width + column;
            field->columns[index] = rowStr[column];
        }
    }
}

void printField(Field* field) {
    printf("ROWS:   ");
    for(unsigned int j = 0 ; j < field->width; j++) {
        printf("%i", j);
    }
    printf("\n");
    for(unsigned int j = 0; j < field->width ; j++) {
        printf("ROW %i:  ", j);
        for(unsigned int i = 0 ; i < field->height ; i++) {
            printf("%c", getColumn(field, j, i));
        }
        printf("\n");
    }
}

char getColumn(Field* field, unsigned int x, unsigned int y) {
    return field->columns[x * field->width + y];
}

void freeField(Field* field) {
    free(field->columns);
    free(field);
}

bool isValidMove(Field* field, char own, unsigned int x, unsigned int y) {
    char current = getColumn(field, x, y);

    if(current == 'W' || current == 'B') {
        printf("%i %i failed own check (current is %c)", x, y, current);
        return false;
    }


    char other = own == 'W' ? 'B' : 'W';

    // vertical check ^
    long int found = -1;
    for(unsigned int i = y; i >= 0; i--) {
        char column = getColumn(field, x, i);

        if(column == '*' && i != y) {
            break;
        }

        if(column == other) {
            found = i;
            continue;
        }

        if(column == own && found != -1)
            return true;
    }
    found = -1;

    for(unsigned int i = y; i < field->height; i++) {
        char column = getColumn(field, x, i);

        if(column == '*' && i != y)
            break;

        if(column == other) {
            found = i;
            continue;
        }

        if(column == own && found != -1)
            return true;
    }
    found = -1;

    for(unsigned int j = x; j >= 0; j--) {
        char column = getColumn(field, j, y);

        if(column == '*' && j != x)
            break;

        if(column == other) {
            found = j;
            continue;
        }

        if(column == own && found != -1)
            return true;
    }
    found = -1;

    // horizontal check >
    for(unsigned int j = x; j < field->width; j++) {
        char column = getColumn(field, j, y);

        if(column == '*' && j != x)
            break;

        if(column == other) {
            found = j;
            continue;
        }

        if(column == own && found != -1)
            return true;
    }
    found = -1;

    // diagonal check <^
    for(unsigned int i = y, j = x; i >= 0 && j >= 0; j--, i--) {
        char column = getColumn(field, j, i);

        if(column == '*' && i != y)
            break;

        if(column == other) {
            found = j;
            continue;
        }

        if(column == own && found != -1)
            return true;
    }

    // diagonal check |>
    for(unsigned int i = y, j = x; i < field->height && j < field->width; j++, i++) {
        char column = getColumn(field, j, i);

        if(column == '*' && i != y)
            break;

        if(column == other) {
            found = j;
            continue;
        }

        if(column == own && found != -1)
            return true;
    }

    return false;
}