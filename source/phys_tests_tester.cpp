#include "compliance_interface.h"
#include "jacdac_tests.h"

PHYS_TEST(0)
{
    int lineVal = get_jacdac_gpio(JACDAC_GPIO_PULL_MODE_NONE);

    if (lineVal == 1)
        return 0;

    return -1;
}