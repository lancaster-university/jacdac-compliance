#include "compliance_interface.h"
#include "jacdac_tests.h"
#include "CodalDmesg.h"

#ifdef DEVICE_TESTER

int device_main()
{
    device_init();

    set_reset_gpio(LINE_ACTIVE_VALUE);
    wait_us(100);
    set_reset_gpio(!LINE_ACTIVE_VALUE);
    wait_ms(1);
    while(get_tx_rx_gpio() != LINE_ACTIVE_VALUE);
    set_test_status(1);
    wait_ms(1);
    DMESG("AFTER");
    jacdac_init();

    int results[JACDAC_TEST_COUNT];

    for (int i = 0; i < JACDAC_TEST_COUNT; i++)
    {
        results[i] = (*jacdac_tests[i])();
    }

    for (int i = 0; i < JACDAC_TEST_COUNT; i++)
    {
        DMESG("T%d: %d", i, results[i]);
    }
    return 0;
}

#endif