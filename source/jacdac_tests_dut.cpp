#include "compliance_interface.h"
#include "jacdac_tests.h"
#include "CodalDmesg.h"

#ifdef DEVICE_UNDER_TEST

volatile int tx_rx = 0;
volatile int error = 0;

void on_reset_gpio_high()
{
    while(1);
    device_reset();
}

void on_tx_rx_gpio_high()
{
    tx_rx = 1;
}

void on_error_gpio_high()
{
    error = 1;
}

int packet_received_from_tester()
{
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(10);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    return 0;
}

int send_packet_to_tester()
{
    while(get_tx_rx_gpio() == !LINE_ACTIVE_VALUE);
    int testNumber = 1;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));
    return 0;
}

int (*jacdac_tests[JACDAC_TEST_COUNT])() =
{
    packet_received_from_tester,
    send_packet_to_tester
};


#endif
