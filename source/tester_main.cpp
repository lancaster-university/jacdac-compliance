#include "compliance_interface.h"
#include "jacdac_tests.h"
#include "CodalDmesg.h"

#ifdef DEVICE_TESTER

extern int phys_test_len;
extern int jacdac_test_len;

int device_main()
{
    device_init();

    set_reset_gpio(LINE_ACTIVE_VALUE);
    wait_us(100);
    set_reset_gpio(!LINE_ACTIVE_VALUE);
    wait_ms(20);
    while(get_tx_rx_gpio(JACDAC_GPIO_PULL_MODE_UP) != LINE_ACTIVE_VALUE);
    while(get_tx_rx_gpio(JACDAC_GPIO_PULL_MODE_UP) == LINE_ACTIVE_VALUE);
    set_test_status(1);
    wait_ms(1);
    DMESG("AFTER");

    jacdac_init();

    DMESG("PhysLen: %d", phys_test_len);

    int phys_results[phys_test_len];

    for (int i = 0; i < phys_test_len; i++)
    {
        phys_results[i] = (*phys_tests[i])();
    }

    DMESG("PHYS RESULTS:\r\n");
    for (int i = 0; i < phys_test_len; i++)
    {
        DMESG("T%d: %d, ", i, phys_results[i]);
    }

    int jacdac_results[jacdac_test_len];

    for (int i = 0; i < jacdac_test_len; i++)
    {
        jacdac_results[i] = (*jacdac_tests[i])();
    }

    DMESG("JACDAC RESULTS:\r\n");
    for (int i = 0; i < jacdac_test_len; i++)
    {
        DMESG("T%d: %d, ", i, jacdac_results[i]);
    }
    return 0;
}

#endif