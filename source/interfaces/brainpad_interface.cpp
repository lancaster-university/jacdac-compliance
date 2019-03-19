#include "compliance_interface.h"

#if (DEVICE_TESTER == CODAL_STM32_BRAINPAD || DEVICE_UNDER_TEST == CODAL_STM32_BRAINPAD) // set in device_tester.h

#include "BrainPad.h"

using namespace codal;

BrainPad bp;

// ZSingleWireSerial serialOut(bp.io.d1);

#define DEVICE_TESTER_PIN_RESET     bp.io.d3
#define DEVICE_TESTER_PIN_TX_RX     bp.io.d4
#define DEVICE_TESTER_PIN_ERROR     bp.io.d5

PullMode translate_to_pullmode(int pm)
{
    if (pm == JACDAC_GPIO_PULL_MODE_UP)
        return PullMode::Up;
    if (pm == JACDAC_GPIO_PULL_MODE_NONE)
        return PullMode::None;
    if (pm == JACDAC_GPIO_PULL_MODE_DOWN)
        return PullMode::Down;

    return PullMode::Up;
}

extern "C" void set_gpio(int val)
{
    bp.io.d1.setDigitalValue(val);
}

void set_gpio2(int val)
{
    bp.io.d2.setDigitalValue(val);
}

void set_gpio3(int val)
{
    bp.io.d6.setDigitalValue(val);
}

void set_gpio4(int val)
{
    bp.io.d7.setDigitalValue(val);
}

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

void on_jacdac_error(Event)
{
    set_error_gpio(1);
    wait_us(40);
    set_error_gpio(0);
}

void configure_error_interrupt(bool enable)
{
    if (enable)
    {
        DEVICE_TESTER_PIN_ERROR.disconnect();
        bp.messageBus.listen(DEVICE_TESTER_PIN_ERROR.id, DEVICE_PIN_EVT_RISE, on_error_high, MESSAGE_BUS_LISTENER_IMMEDIATE);
        DEVICE_TESTER_PIN_ERROR.eventOn(DEVICE_PIN_EVENT_ON_EDGE);
    }
    else
    {
        bp.messageBus.ignore(DEVICE_TESTER_PIN_ERROR.id, DEVICE_PIN_EVT_RISE, on_error_high);
        DEVICE_TESTER_PIN_ERROR.eventOn(DEVICE_PIN_EVENT_NONE);
    }
}
void configure_tx_rx_interrupt(bool enable)
{
    if (enable)
    {
        DEVICE_TESTER_PIN_TX_RX.disconnect();
        bp.messageBus.listen(DEVICE_TESTER_PIN_TX_RX.id, DEVICE_PIN_EVT_RISE, on_tx_rx_high, MESSAGE_BUS_LISTENER_IMMEDIATE);
        DEVICE_TESTER_PIN_TX_RX.eventOn(DEVICE_PIN_EVENT_ON_EDGE);
    }
    else
    {
        bp.messageBus.ignore(DEVICE_TESTER_PIN_TX_RX.id, DEVICE_PIN_EVT_RISE, on_tx_rx_high);
        DEVICE_TESTER_PIN_TX_RX.eventOn(DEVICE_PIN_EVENT_NONE);
    }
}

void configure_reset_interrupt(bool enable)
{
    if (enable)
    {
        DEVICE_TESTER_PIN_RESET.disconnect();
        bp.messageBus.listen(DEVICE_TESTER_PIN_RESET.id, DEVICE_PIN_EVT_RISE, on_reset_high, MESSAGE_BUS_LISTENER_IMMEDIATE);
        DEVICE_TESTER_PIN_RESET.eventOn(DEVICE_PIN_EVENT_ON_EDGE);
    }
    else
    {
        bp.messageBus.ignore(DEVICE_TESTER_PIN_RESET.id, DEVICE_PIN_EVT_RISE, on_reset_high);
        DEVICE_TESTER_PIN_RESET.eventOn(DEVICE_PIN_EVENT_NONE);
    }
}

void device_init()
{
    bp.init();
    bp.io.d1.setDigitalValue(0);
    DEVICE_TESTER_PIN_RESET.setDigitalValue(0);
    DEVICE_TESTER_PIN_TX_RX.setDigitalValue(0);
    DEVICE_TESTER_PIN_ERROR.setDigitalValue(0);
    set_gpio(0);
    set_gpio2(0);
    set_gpio3(0);
    set_gpio4(0);
}

void device_reset()
{
    target_reset();
}

void jacdac_init()
{
    bp.jacdac.start();
    bp.messageBus.listen(bp.jacdac, JD_SERIAL_EVT_BUS_ERROR, on_jacdac_error, MESSAGE_BUS_LISTENER_IMMEDIATE);
    bp.messageBus.listen(bp.jacdac, JD_SERIAL_EVT_CRC_ERROR, on_jacdac_error, MESSAGE_BUS_LISTENER_IMMEDIATE);
}

void jacdac_protocol_init()
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
    bp.jacdac.send(buf, len, 255, 0, JDBaudRate::Baud1M);
}

JDPacket* jacdac_receive()
{
    return bp.jacdac.getPacket();
}

void set_jacdac_gpio(int value)
{
    bp.sws.p.setDigitalValue(value);
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

int get_reset_gpio(int pm)
{
    return DEVICE_TESTER_PIN_RESET.getDigitalValue(translate_to_pullmode(pm));
}

int get_tx_rx_gpio(int pm)
{
    return DEVICE_TESTER_PIN_TX_RX.getDigitalValue(translate_to_pullmode(pm));
}

int get_error_gpio(int pm)
{
    return DEVICE_TESTER_PIN_ERROR.getDigitalValue(translate_to_pullmode(pm));
}

int get_jacdac_gpio(int pm)
{
    return bp.sws.p.getDigitalValue(translate_to_pullmode(pm));
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