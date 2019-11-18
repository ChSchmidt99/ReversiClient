#include <stdio.h>
#include "../include/minunit.h"
#include "./test_utilities.c"
 
 int tests_run = 0;

 static char * all_tests() {
     mu_run_test_no_count(utilities_tests);
     return 0;
 }
 
 int main() {
     char *result = all_tests();
     if (result != 0) {
         printf("%s\n", result);
     } else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n", tests_run);

     return result != 0;
 }