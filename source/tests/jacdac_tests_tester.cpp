#include "compliance_interface.h"
#include "jacdac_tests.h"
#include "CodalDmesg.h"

#ifdef DEVICE_TESTER

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
    DMESG("ERR!!");
    error = 1;
}

int read_tx_rx_gpio()
{
    int res = tx_rx;
    tx_rx = 0;
    return res;
}

int read_error_gpio()
{
    int res = error;
    error = 0;
    return res;
}

JACDAC_TEST(0)
{
    int lineVal = get_jacdac_gpio(JACDAC_GPIO_PULL_MODE_NONE);

    if (lineVal == 1)
        return 0;

    return -1;
}

// send normal packet and wait for dut to signal reception
JACDAC_TEST(1)
{
    int testNumber = 1;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));

    read_tx_rx_gpio();
    configure_tx_rx_interrupt(true);
    wait_ms(20);
    configure_tx_rx_interrupt(false);

    int res = read_tx_rx_gpio();
    return (res > 0) ? 0 : -1;
}

// ask dut to send us a packet
JACDAC_TEST(2)
{
    int testNumber = 2;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));
    wait_ms(5);
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(10);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);

    JDPacket* rx = NULL;
    wait_ms(20);
    rx = jacdac_receive();

    if (rx)
    {
        int rxNumber = *(int*)rx->data;
        delete rx;

        if (rxNumber == testNumber)
            return 0;
    }

    return -1;
}

// send packet, broken checksum
JACDAC_TEST(3)
{
    JDPacket* packet = new JDPacket;
    memset(packet, 0, sizeof(JDPacket));

    packet->address = 255;
    packet->size = 4;
    int* data = (int*)packet->data;
    *data = 3;

    read_error_gpio();

    configure_error_interrupt(true);
    jacdac_send_no_crc(packet);
    wait_ms(1000);
    delete packet;
    configure_error_interrupt(false);

    int res = read_error_gpio();
    return (res == 1) ? 0 : -1;
}

// bus lo, wrong amount of time.
JACDAC_TEST(4)
{
    read_error_gpio();

    configure_error_interrupt(true);
    set_jacdac_gpio(0);
    wait_ms(1);
    get_jacdac_gpio(JACDAC_GPIO_PULL_MODE_UP);
    configure_error_interrupt(false);

    int error_triggered = read_error_gpio();

    int testNumber = 4;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));

    read_tx_rx_gpio();
    configure_error_interrupt(true);
    configure_tx_rx_interrupt(true);
    wait_ms(20);
    configure_tx_rx_interrupt(false);
    configure_error_interrupt(false);

    int packet_received = read_tx_rx_gpio();
    int error_triggered_rx = read_error_gpio();

    return (error_triggered && packet_received && !error_triggered_rx) ? 0 : -1;
}

// bus lo, Long time
JACDAC_TEST(5)
{
    read_error_gpio();

    configure_error_interrupt(true);
    set_jacdac_gpio(0);
    wait_ms(1000);
    get_jacdac_gpio(JACDAC_GPIO_PULL_MODE_UP);
    configure_error_interrupt(false);

    int error_triggered = read_error_gpio();

    int testNumber = 5;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));

    read_tx_rx_gpio();
    configure_error_interrupt(true);
    configure_tx_rx_interrupt(true);
    wait_ms(20);
    configure_tx_rx_interrupt(false);
    configure_error_interrupt(false);

    int packet_received = read_tx_rx_gpio();
    int error_triggered_rx = read_error_gpio();

    return (error_triggered && packet_received && !error_triggered_rx) ? 0 : -1;
}

// bus lo 10 us, wait 40 us, toggle line at 10 us (trigger error), send packet.
JACDAC_TEST(6)
{
    read_error_gpio();

    configure_error_interrupt(true);
    set_jacdac_gpio(0);
    wait_us(10);
    set_jacdac_gpio(1);
    wait_us(40);

    for(int i = 0; i < 20; i++)
    {
        set_jacdac_gpio(i % 2);
        wait_us(10);
    }

    configure_error_interrupt(false);

    int error_triggered = read_error_gpio();

    int testNumber = 6;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));

    read_tx_rx_gpio();
    configure_error_interrupt(true);
    configure_tx_rx_interrupt(true);
    wait_ms(20);
    configure_tx_rx_interrupt(false);
    configure_error_interrupt(false);

    int packet_received = read_tx_rx_gpio();
    int error_triggered_rx = read_error_gpio();

    return (error_triggered && packet_received && !error_triggered_rx) ? 0 : -1;
}

// bus lo 10 us, wait 40 us, toggle line at 7 us 3 times, then stop. Send packet later.
JACDAC_TEST(7)
{
    read_error_gpio();

    configure_error_interrupt(true);
    set_jacdac_gpio(0);
    wait_us(10);
    set_jacdac_gpio(1);
    wait_us(40);

    for(int i = 0; i < 3; i++)
    {
        set_jacdac_gpio(0);
        wait_us(6);
        set_jacdac_gpio(1);
        wait_us(1);
    }

    wait_us(400);
    configure_error_interrupt(false);

    get_jacdac_gpio(JACDAC_GPIO_PULL_MODE_UP);

    int error_triggered = read_error_gpio();

    int testNumber = 7;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));

    read_tx_rx_gpio();
    configure_error_interrupt(true);
    configure_tx_rx_interrupt(true);
    wait_ms(20);
    configure_tx_rx_interrupt(false);
    configure_error_interrupt(false);

    int packet_received = read_tx_rx_gpio();
    int error_triggered_rx = read_error_gpio();

    return (error_triggered && packet_received && !error_triggered_rx) ? 0 : -1;
}

// bus lo 10 us, wait 40 us, toggle line at 7 us 3 times, then stop, no delay send packet.
JACDAC_TEST(8)
{
    read_error_gpio();


    configure_error_interrupt(true);

    set_jacdac_gpio(0);
    wait_us(10);
    set_jacdac_gpio(1);
    wait_us(40);

    // because we leave no time gap
    // an error should be generated after we send the lo pulse for this packet
    int testNumber = 8;
    // we have to send before our dummy writes, because there's a 160 us backoff in codal imp.
    jacdac_send((uint8_t*)&testNumber, sizeof(int));

    for(int i = 0; i < 3; i++)
    {
        set_jacdac_gpio(0);
        wait_us(6);
        set_jacdac_gpio(1);
        wait_us(1);
    }

    read_tx_rx_gpio();
    configure_error_interrupt(true);
    configure_tx_rx_interrupt(true);
    wait_ms(20);
    configure_tx_rx_interrupt(false);
    configure_error_interrupt(false);

    int error_triggered_rx = read_error_gpio();

    return (error_triggered_rx) ? 0 : -1;
}

// bus lo 10 us, never send data, send packet after.
JACDAC_TEST(9)
{
    read_error_gpio();

    configure_error_interrupt(true);
    set_jacdac_gpio(0);
    wait_us(10);
    set_jacdac_gpio(1);
    wait_us(350);
    configure_error_interrupt(false);

    int error_triggered = read_error_gpio();

    int testNumber = 9;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));

    read_tx_rx_gpio();
    configure_error_interrupt(true);
    configure_tx_rx_interrupt(true);
    wait_ms(20);
    configure_tx_rx_interrupt(false);
    configure_error_interrupt(false);

    int packet_received = read_tx_rx_gpio();
    int error_triggered_rx = read_error_gpio();

    return (error_triggered && packet_received && !error_triggered_rx) ? 0 : -1;
}

// hold line low, ask device to send...
JACDAC_TEST(10)
{
    read_error_gpio();

    int testNumber = 10;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));
    wait_ms(20);
    configure_error_interrupt(true);
    set_jacdac_gpio(!LINE_ACTIVE_VALUE);
    set_tx_rx_gpio(LINE_ACTIVE_VALUE);
    wait_us(20);
    set_tx_rx_gpio(!LINE_ACTIVE_VALUE);
    wait_ms(20);
    configure_error_interrupt(false);
    get_jacdac_gpio(JACDAC_GPIO_PULL_MODE_UP);

    int error_triggered = read_error_gpio();

    return (error_triggered) ? 0 : -1;
}

#endif