#include "compliance_interface.h"
#include "jacdac_tests.h"
#include "CodalDmesg.h"

#ifdef DEVICE_UNDER_TEST

volatile int tx_rx = 0;
volatile int error = 0;

void on_reset_gpio_high()
{
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

JACDAC_TEST(0)
{
    // empty, no packet transferred.
    return 0;
}

/**
 * packet_received_from_tester
 *
 * toggles the tx_rx line to indicate a packet has been received.
 **/
JACDAC_TEST(1)
{
    DMESG("TEST 1");
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(10);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    return 0;
}

/**
 * send_packet_to_tester
 * Sends a jacdac packet to the tester.
 **/
JACDAC_TEST(2)
{
    DMESG("TEST 2");
    while(get_tx_rx_gpio(JACDAC_GPIO_PULL_MODE_DOWN) == !LINE_ACTIVE_VALUE);
    int testNumber = 2;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));
    return 0;
}

/**
 * packet with an incorrect crc sent from tester
 * toggles the error line to indicate a packet has been received.
 **/
JACDAC_TEST(3)
{
    // empty, error should trigger the next test
    return 0;
}

/**
 * bus is driven low for an incorrect amount of time, then a packet is sent.
 *
 * This tests error detection and recovery.
 **/
JACDAC_TEST(4)
{
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(10);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    return 0;
}

/**
 * bus is driven low for a really long amount of time, then a packet is sent.
 *
 * This tests error detection and recovery.
 **/
JACDAC_TEST(5)
{
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(10);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    return 0;
}

/**
 * bus is driven low for 10 us, wait 40 us, send junk. After erroring, tester sends packet.
 *
 * This tests error detection and recovery.
 **/
JACDAC_TEST(6)
{
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(10);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    return 0;
}

/**
 * bus is driven low for 10 us, wait 40 us, send 3 junk bytes, (header timeout test)
 *
 * This tests error detection and recovery.
 **/
JACDAC_TEST(7)
{
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(10);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    return 0;
}

/**
 * bus is driven low for 10 us, line twiddled for three bytes, then an actual packet is sent immediately
 *
 * This tests error detection and recovery.
 **/
JACDAC_TEST(8)
{
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(10);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    return 0;
}

/**
 * bus is driven low for 10 us, packet after 300 us
 *
 * This tests error detection and recovery.
 **/
JACDAC_TEST(9)
{
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(10);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    return 0;
}

#endif
