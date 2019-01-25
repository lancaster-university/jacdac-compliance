#include "jacdac_tests.h"

PHYS_TEST_EXTERN(0);

int (*phys_tests[])() =
{
    PHYS_TEST_PTR(0)
};

int phys_test_len = sizeof(phys_tests) / sizeof(phys_tests[0]);

JACDAC_TEST_EXTERN(0);
JACDAC_TEST_EXTERN(1);


int (*jacdac_tests[])() = {
    JACDAC_TEST_PTR(0),
    JACDAC_TEST_PTR(1),
};

int jacdac_test_len = sizeof(jacdac_tests) / sizeof(jacdac_tests[0]);