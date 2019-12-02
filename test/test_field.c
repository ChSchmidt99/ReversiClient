#include "minunit.h"
#include "thinker/field.h"

#include <string.h>
#include <stdlib.h>

static char *testField() {
    Field *field = createField(8, 8);
    char *data[] = {"********", "********", "********", "***WB***", "***BW***", "********", "********", "********"};
    loadField(field, data);

    mu_assert("Should be invalid move", isValidMove(field, 'W', 3, 2) == false);
    mu_assert("Should be valid move", isValidMove(field, 'B', 3, 2) == true);

    char *data2[] = {"********", "********", "***B****", "***BB***", "***BW***", "********", "********", "********"};
    loadField(field, data2);
    mu_assert("Should be invalid move 2", isValidMove(field, 'B', 2, 2) == false);
    mu_assert("Should be valid move 2", isValidMove(field, 'W', 2, 2) == true);

    char* str = toString(3, 4);
    mu_assert("String should be equal", strcmp(str, "D5") == 0);
    free(str);

    freeField(field);
    return 0;
}

static char *field_tests() {
    mu_run_test(testField);
    return 0;
}