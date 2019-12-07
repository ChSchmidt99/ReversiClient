#include "minunit.h"
#include <string.h>
#include "../src/communicator/gamesequence_priv.h"

static char * test_boarddimension() {
    int rows = 0;
    int cols = 0;
    receiveBoardDimensions(NULL,&rows, &cols);
    printf("Rows: %i\n",rows);
    printf("Cols: %i\n",cols);
    return 0;
}

static char * utilities_tests() {
    mu_run_test(test_boarddimension);
    return 0;
}
 