#include <stdio.h>
#include "../include/minunit.h"
#include "./test_utilities.c"
#include "./test_config.c"
#include "./test_field.c"
#include "./test_shared_memory.c"

 int tests_run = 0;

 static char * all_tests() {
     mu_run_test_no_count(utilities_tests);
     mu_run_test_no_count(config_tests);
     mu_run_test_no_count(field_tests);
     mu_run_test_no_count(shared_memory_tests);
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