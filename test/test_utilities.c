#include "minunit.h"
#include "utilities.h"
#include <string.h>


static char * testConcatStringsToNewMemoryAddr() {
    char* str1 = "Hallo";
    char* str2 = "Welt";
    char* newString = concatStringToNewMemoryAddr(str1,str2," ");
    mu_assert("Strings Should be Equal", strcmp("Hallo Welt",newString) == 0);
    return 0;
}

static char * test_copyStringToNewMemoryAddr() {
    char* baseString = "3,2";
    char* newString = copyStringToNewMemoryAddr(baseString);
    mu_assert("Strings should be Equal", strcmp(baseString,newString) == 0);
    free(newString);
    return 0;
}

static char * test_Slice() {
    size_t length = 0;
    char** tokens = slice("Hallo ich bin ein toller test"," ",&length);
    mu_assert("Should have 6 tokens", length == 6);
    mu_assert("First Token should be Hallo", strcmp(tokens[0],"Hallo") == 0);
    return 0;
}

static char * utilities_tests() {
    mu_run_test(testConcatStringsToNewMemoryAddr);
    mu_run_test(test_copyStringToNewMemoryAddr);
    mu_run_test(test_Slice);
    return 0;
}
 