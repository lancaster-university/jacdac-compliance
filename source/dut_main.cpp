#include "compliance_interface.h"
#include "jacdac_tests.h"
#include "CodalDmesg.h"

#ifdef DEVICE_UNDER_TEST

extern int jacdac_test_len;
int device_main()
{
    device_init();

    // configure to reset the device when the line is driven high
    configure_reset_interrupt(true);
    set_test_status(0);
    wait_us(500000);
    set_test_status(1);
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(100);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    jacdac_init();

    DMESG("ALIVE");

    JDPacket* packet = NULL;
    while(1)
    {
        while((packet = jacdac_receive()) == NULL);
        int testNumber = *((int *)packet->data);

        if (testNumber < jacdac_test_len)
            (*jacdac_tests[testNumber])();

        delete packet;
        packet = NULL;
    }
}

#endif