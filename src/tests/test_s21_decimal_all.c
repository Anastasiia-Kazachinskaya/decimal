#include <check.h>
#include <stdlib.h> 
#include "../headers/s21_helpers.h"

Suite* s21_helpers_suite(void);

int main(void) {
    int failed = 0;
    
    Suite* suites[] = {
        s21_helpers_suite(),
        NULL
    };
    
    for (int i = 0; suites[i] != NULL; i++) {
        SRunner* sr = srunner_create(suites[i]);
        srunner_run_all(sr, CK_NORMAL);
        failed += srunner_ntests_failed(sr);
        srunner_free(sr);
    }
    
    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
