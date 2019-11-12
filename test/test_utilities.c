#include "../include/minunit.h"
#include "../include/utilities.h"
#include <string.h>


static char * testConcatStringsToNewMemoryAddr() {
    char* str1 = "Hallo";
    char* str2 = "Welt";
    char* newString = concatStringToNewMemoryAddr(str1,str2," ");
    mu_assert("Strings Should be Equal", strcmp("Hallo Welt",newString));
    return 0;
}

static char * utilities_tests() {
    mu_run_test(testConcatStringsToNewMemoryAddr);
    return 0;
}
 