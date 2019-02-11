#ifndef JACDAC_TESTS_H
#define JACDAC_TESTS_H

#define JACDAC_TEST(n) int jacdac_test_## n ()

#define JACDAC_TEST_PTR(n) jacdac_test_## n
#define JACDAC_TEST_EXTERN(n) extern int jacdac_test_## n ()

extern int (*jacdac_tests[])();

#endif