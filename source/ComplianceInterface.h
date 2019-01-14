#ifndef COMPLIANCE_INTERFACE_H
#define COMPLIANCE_INTERFACE_H

#define DEVICE_TESTER
// #define DEVICE_UNDER_TEST

void jacdac_send(uint8_t* buf, int len);

void set_reset_gpio(int);

void set_tx_rx_gpio(int);

void set_error_gpio(int);

void wait_us(uint32_t);

void device_main();

#endif