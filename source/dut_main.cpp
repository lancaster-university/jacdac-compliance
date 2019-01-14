#include "compliance_interface.h"
#include "jacdac_tests.h"
#include "CodalDmesg.h"

#ifdef DEVICE_UNDER_TEST

int device_main()
{
    device_init();

    wait_us(10000);
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(10);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    set_test_status(1);

    JDPacket* packet = NULL;
    while(1)
    {
        while((packet = jacdac_receive()) == NULL);
        DMESG("RECV");
        int* testNumber = (int *)packet->data;
        (*jacdac_tests[*testNumber])();
    }
}

#endif