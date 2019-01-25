# JACDAC Compliance Testing

This repository is the ground truth on how JACDAC should behave. It subjects a JACDAC device to a number of tests with increasing complexity.

## Device requirements

* 3 GPIOs
    - Reset line –– Used to trigger a reset (in some cases this might be required).
    - tx/rx line –– Used for two purposes: (1) acknowledge a packet has been received; (2) cause a packet to be sent from the device under test (DUT).
    - Error line –– When toggled to one, an error has occurred.
* Serial output (for outputting test results). The interface simply expects a buffer, so alternate output mechanisms can be used instead.
* A verified and JACDAC capable device.

# Physical layer tests

## Tester checks the TX line

### Test Outline
1. The tester reads the default state of the tx line with NO PULL.

### Expected Result
The tester should expect the tx line to be hi.


# Transmission / Reception Tests

## Tester sends a JDPacket

### Test Outline
1. Tester sends a packet to the DUT containing the test number.
2. DUT receives the packet and processes the test number
3. DUT toggles the tx/rx line to indicate the packet has been received correctly.

### Expected Result
The tester should expect the tx/rx line to be toggled, and set the result to 0 (a success).

## DUT sends a JDPacket

### Test Outline
1. Tester sends a packet to the DUT containing the test number.
2. DUT receives the packet and processes the test number
3. DUT sends JDPacket with the test number to the TESTER.

### Expected Result
The tester should expect to receive a packet and set the result to 0 (a success).

## Tester pulses the TX line

### Test Outline
1. Tester toggles the JACDAC TX line
2. DUT toggles the error line to indicate an error has been observed on the bus.

### Expected Result
The tester should expect the error line to be toggled, and set the result to 0 (a success).

## Tester sends a JDPacket with an incorrect checksum

### Test Outline
1. Tester sends a packet to the DUT with an incorrect crc
2. DUT toggles the error line to indicate the packet has not been received.

### Expected Result
The tester should expect the error line to be toggled, and set the result to 0 (a success).

## Tester sends a JDPacket with an incorrect size

This test observes if a device meets the maximum timeout permitted on the JACDAC bus. If a device is expecting to receive more bytes than it's given, the device should timeout as per the JACDAC specification.

### Test Outline
1. Tester sends a packet to the DUT with a size bigger than the number of transmitted bytes
2. DUT toggles the error line to indicate the packet has not been received.

### Expected Result
The tester should expect the error line to be toggled, and set the result to 0 (a success).

## Tester sends a JDPacket and triggers a UART error condition

This test observes if a device identifies UART error conditions

### Test Outline
1. Tester sends a JDPacket header to the DUT and drives the line low.
2. DUT toggles the error line to indicate the packet has not been received.

### Expected Result
The tester should expect the error line to be toggled, and set the result to 0 (a success).


## Time the accuracy of the lo pulse

### Test Outline
1. Tester toggles tx/rx line
2. DUT sends a packet.
3. The LO pulse is timed for accuracy

### Expected Result
If the pulse is within tolerance (+/- 20%) of the current baud rate. (This means slower devices have a wider tolerance).
