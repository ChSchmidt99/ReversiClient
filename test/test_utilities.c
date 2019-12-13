#include "misc/minunit.h"
#include "misc/utilities.h"
#include <string.h>


static char * testConcatStringsToNewMemoryAddr() {
    char* str1 = "Hallo";
    char* str2 = "Welt";
    char* newString = concatStringToNewMemoryAddr(str1,str2," ");
    mu_assert("testConcatStringsToNewMemoryAddr failed", strcmp("Hallo Welt",newString) == 0);
    return 0;
}

static char * test_copyStringToNewMemoryAddr() {
    char* baseString = "3,2";
    char* newString = copyStringToNewMemoryAddr(baseString);
    mu_assert("test_copyStringToNewMemoryAddr failed", strcmp(baseString,newString) == 0);
    free(newString);
    return 0;
}


static char * test_Slice() {
    size_t length = 0;
    char* in = "Hallo ich bin ein toller test";
    char** tokens = slice(in," ",&length);
    mu_assert("test_Slice failed, Should have 6 tokens", length == 6);
    mu_assert("test_Slice failed, First Token should be Hallo", strcmp(tokens[0],"Hallo") == 0);
    freeTokens(tokens);
    return 0;
}

/*
static char * test_SliceLimit() {
    size_t length = 0;
    char* in = "Hallo ich bin ein toller test";
    char** tokens = sliceLimit(in, " ", &length, 3);
    mu_assert("test_SliceLimit failed, Should have 3 tokens", length == 3);
    mu_assert("test_SliceLimit failed, Last Token should be 'bin ein toller test'", strcmp(tokens[2], "bin ein toller test") == 0);
    freeTokens(tokens);
    return 0;
}
*/

static char* test_freeArray(){
    char** testArr = safeMalloc(sizeof(char**) * 2);
    testArr[0] = safeMalloc(sizeof(char) * 5);
    testArr[1] = safeMalloc(sizeof(char) * 5);
    freeArrayWithContents((void**)testArr,2);
    return 0;
}

static char* test_joinTokens(){
    char* strs[3] = {"Hallo","schöne","Welt"};
    char* result = joinTokens(strs,3," ");
    mu_assert("test_joinTokens failed", strcmp(result,"Hallo schöne Welt") == 0);
    free(result);
    return 0;
}

static char* test_joinTokens_single_token(){
    char* strs[1] = {"Hallo"};
    char* result = joinTokens(strs,1," ");
    mu_assert("test_joinTokens_single_token failed", strcmp(result,"Hallo") == 0);
    free(result);
    return 0;
}

static char* test_remove_delimiter(){
    char* newString = newStringWithoutDelimiter("Ich bin ein Test", ' ');
    mu_assert("test_remove_delimiter failed",strcmp(newString,"IchbineinTest") == 0);
    free(newString);
    return 0;
}

static char * utilities_tests() {
    mu_run_test(testConcatStringsToNewMemoryAddr);
    mu_run_test(test_copyStringToNewMemoryAddr);
    mu_run_test(test_Slice);
    mu_run_test(test_freeArray);
    mu_run_test(test_joinTokens);
    mu_run_test(test_joinTokens_single_token);
    mu_run_test(test_remove_delimiter);
    //mu_run_test(test_SliceLimit);
    return 0;
}
 