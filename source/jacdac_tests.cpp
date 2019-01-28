#include "jacdac_tests.h"

JACDAC_TEST_EXTERN(0);
JACDAC_TEST_EXTERN(1);
JACDAC_TEST_EXTERN(2);
JACDAC_TEST_EXTERN(3);
JACDAC_TEST_EXTERN(4);
JACDAC_TEST_EXTERN(5);
JACDAC_TEST_EXTERN(6);


int (*jacdac_tests[])() = {
    JACDAC_TEST_PTR(0),
    JACDAC_TEST_PTR(1),
    JACDAC_TEST_PTR(2),
    JACDAC_TEST_PTR(3),
    JACDAC_TEST_PTR(4),
    JACDAC_TEST_PTR(5),
    JACDAC_TEST_PTR(6),
};

int jacdac_test_len = sizeof(jacdac_tests) / sizeof(jacdac_tests[0]);