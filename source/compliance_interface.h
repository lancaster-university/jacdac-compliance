#ifndef COMPLIANCE_INTERFACE_H
#define COMPLIANCE_INTERFACE_H

#include "JACDAC.h"

// we want to be abstraction layer neutral eventually.
#ifdef CODAL_JACDAC_H
typedef codal::JDPacket JDPacket;
#endif

#include <stdint.h>
#include <stdio.h>

// #define DEVICE_TESTER CODAL_STM32_BRAINPAD
#define DEVICE_UNDER_TEST CODAL_STM32_BRAINPAD

#ifdef DEVICE_TESTER
#pragma message "DEVICE_TESTER"
#else
#pragma message "DEVICE_UNDER_TEST"
#endif

#define LINE_ACTIVE_VALUE    1

#define JACDAC_GPIO_PULL_MODE_UP        0
#define JACDAC_GPIO_PULL_MODE_NONE      1
#define JACDAC_GPIO_PULL_MODE_DOWN      2

void serial_tx(uint8_t* buf, int len);

void jacdac_init();
void jacdac_protocol_init();
void jacdac_send_no_crc(JDPacket*);
void jacdac_send(JDPacket*);
void jacdac_send(uint8_t* buf, int len);
JDPacket* jacdac_receive();

void set_jacdac_gpio(int);
void set_reset_gpio(int);
void set_tx_rx_gpio(int);
void set_error_gpio(int);

void configure_error_interrupt(bool);
void configure_tx_rx_interrupt(bool);
void configure_reset_interrupt(bool);

void set_test_status(int);

void on_reset_gpio_high();
void on_tx_rx_gpio_high();
void on_error_gpio_high();

int get_jacdac_gpio(int);
int get_reset_gpio(int);
int get_tx_rx_gpio(int);
int get_error_gpio(int);

void wait_ms(uint32_t);
void wait_us(uint32_t);

void device_init();
void device_reset();
int device_main();

#endif