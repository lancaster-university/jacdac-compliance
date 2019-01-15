#include "compliance_interface.h"
#include "jacdac_tests.h"

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

int send_jacdac_packet_to_DUT()
{
    int testNumber = 0;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));

    configure_tx_rx_interrupt(true);
    wait_ms(20);
    configure_tx_rx_interrupt(false);

    int res = read_tx_rx_gpio();
    return (res > 0) ? 0 : -1;
}

int DUT_send_jacdac_packet()
{
    int testNumber = 1;
    jacdac_send((uint8_t*)&testNumber, sizeof(int));
    wait_ms(10);
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

int tester_send_incorrect_chksum()
{
    JDPacket* packet = new JDPacket;
    memset(packet, 0, sizeof(JDPacket));

    packet->address = 255;
    packet->size = 4;
    int* data = (int*)packet->data;
    *data = 4;

    jacdac_send_no_crc(packet);
    delete packet;

    configure_error_interrupt(true);
    wait_ms(20);
    configure_error_interrupt(false);

    int res = read_error_gpio();
    return (res == 1) ? 0 : -1;
}



int (*jacdac_tests[JACDAC_TEST_COUNT])() =
{
    send_jacdac_packet_to_DUT,
    DUT_send_jacdac_packet
};

#endif