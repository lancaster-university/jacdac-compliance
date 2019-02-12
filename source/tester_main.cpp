#include "compliance_interface.h"
#include "jacdac_tests.h"
#include "CodalDmesg.h"

#ifdef DEVICE_TESTER

extern int phys_test_len;
extern int jacdac_test_len;

struct JDResults
{
    uint16_t test_number;
    int16_t result;
};

int device_main()
{
    device_init();

    set_reset_gpio(LINE_ACTIVE_VALUE);
    wait_us(100);
    set_reset_gpio(!LINE_ACTIVE_VALUE);
    wait_ms(20);
    while(get_tx_rx_gpio(JACDAC_GPIO_PULL_MODE_NONE) != LINE_ACTIVE_VALUE);
    while(get_tx_rx_gpio(JACDAC_GPIO_PULL_MODE_NONE) == LINE_ACTIVE_VALUE);
    set_test_status(1);
    wait_ms(1);
    set_test_status(0);
    DMESG("AFTER");

    jacdac_init();

#ifdef SPECIFIC_TEST
    int ret = (*jacdac_tests[SPECIFIC_TEST])();
    if (ret == 0)
        set_test_status(1);
#else

    int success_count = 0;

    JDResults jacdac_results[jacdac_test_len];

    for (int i = 0; i < jacdac_test_len; i++)
    {
        jacdac_results[i].test_number = i;
        jacdac_results[i].result = (*jacdac_tests[i])();

        if (jacdac_results[i].result >= 0)
            success_count++;
    }

    DMESG("JACDAC RESULTS:\r\n");
    for (int i = 0; i < jacdac_test_len; i++)
    {
        DMESG("T%d: %d, ", i, jacdac_results[i]);
    }

    jacdac_send((uint8_t*)jacdac_results,sizeof(jacdac_results));

    if (success_count == jacdac_test_len)
        set_test_status(1);
#endif

    return 0;
}

#endif