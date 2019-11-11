#include <stdio.h>
#include "../include/minunit.h"
 
 int tests_run = 0;

 static char * exampleTest() {
     mu_assert("Should Be True",  1 == 1);
     mu_assert("Should Be False", 1 != 2);
     return 0;
 }

 static char * all_tests() {
     mu_run_test(exampleTest);
     return 0;
 }
 
 int main(int argc, char **argv) {
     char *result = all_tests();
     if (result != 0) {
         printf("%s\n", result);
     } else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n", tests_run);

     return result != 0;
 }