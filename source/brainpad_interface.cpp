#include "compliance_interface.h"

#if (DEVICE_TESTER == CODAL_STM32_BRAINPAD || DEVICE_UNDER_TEST == CODAL_STM32_BRAINPAD) // set in device_tester.h

#include "BrainPad.h"

using namespace codal;

BrainPad bp;

// ZSingleWireSerial serialOut(bp.io.d1);

#define DEVICE_TESTER_PIN_RESET     bp.io.d2
#define DEVICE_TESTER_PIN_TX_RX     bp.io.d4
#define DEVICE_TESTER_PIN_ERROR     bp.io.d5

void on_reset_high(Event)
{
    on_reset_gpio_high();
}

void on_tx_rx_high(Event)
{
    on_tx_rx_gpio_high();
}

void on_error_high(Event)
{
    on_error_gpio_high();
}

void device_init()
{
    bp.init();

    bp.messageBus.listen(DEVICE_TESTER_PIN_RESET.id, DEVICE_PIN_EVT_RISE, on_reset_high, MESSAGE_BUS_LISTENER_IMMEDIATE);
    bp.messageBus.listen(DEVICE_TESTER_PIN_TX_RX.id, DEVICE_PIN_EVT_RISE, on_tx_rx_high, MESSAGE_BUS_LISTENER_IMMEDIATE);
    bp.messageBus.listen(DEVICE_TESTER_PIN_ERROR.id, DEVICE_PIN_EVT_RISE, on_error_high, MESSAGE_BUS_LISTENER_IMMEDIATE);

    DEVICE_TESTER_PIN_RESET.eventOn(DEVICE_PIN_EVENT_ON_EDGE);
    DEVICE_TESTER_PIN_TX_RX.eventOn(DEVICE_PIN_EVENT_ON_EDGE);
    DEVICE_TESTER_PIN_ERROR.eventOn(DEVICE_PIN_EVENT_ON_EDGE);

    bp.jacdac.start();
}

void device_reset()
{
    DMESG("RESET!");
    target_reset();
}

void initialise_jacdac_protocol()
{

}

void jacdac_send_no_crc(JDPacket* packet)
{
    bp.jacdac.send(packet, false);
}

void jacdac_send(JDPacket* packet)
{
    bp.jacdac.send(packet);
}

void jacdac_send(uint8_t* buf, int len)
{
    bp.jacdac.send(buf, len, 255);
}

JDPacket* jacdac_receive()
{
    return bp.jacdac.getPacket();
}

void set_reset_gpio(int value)
{
    DEVICE_TESTER_PIN_RESET.setDigitalValue(value);
}

void set_tx_rx_gpio(int value)
{
    DEVICE_TESTER_PIN_TX_RX.setDigitalValue(value);
}

void set_error_gpio(int value)
{
    DEVICE_TESTER_PIN_ERROR.setDigitalValue(value);
}

void set_test_status(int value)
{
    bp.io.ledGreen.setDigitalValue(value);
}

int get_reset_gpio()
{
    return DEVICE_TESTER_PIN_RESET.getDigitalValue(PullMode::Down);
}

int get_tx_rx_gpio()
{
    return DEVICE_TESTER_PIN_TX_RX.getDigitalValue(PullMode::Down);
}

int get_error_gpio()
{
    return DEVICE_TESTER_PIN_ERROR.getDigitalValue(PullMode::Down);
}

void serial_tx(uint8_t* buf, int len)
{
    // for (int i = 0; i < len; i++)
    //     serialOut.putc(buf[i]);
}

void wait_us(uint32_t value)
{
    system_timer_wait_us(value);
}

void wait_ms(uint32_t value)
{
    system_timer_wait_ms(value);
}

#endif