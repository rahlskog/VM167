/*
 * Public VM167 header file
 * Copyright (C) 2011, Robert Ahlskog <robert.ahlskog@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
 * parameters:
 *   CardAddress, address of device 0 or 1
 * returns: 
 */
#ifndef VM167
#define VM167
#ifdef __cplusplus
extern "C" {
#endif

/* Open devices found on system
 * parameters: none
 * returns: int
 *   -1 no devices found
 *    0 driver problem, try reconnecting
 *    1 device address 0 found
 *    2 device address 1 found
 *    3 device address 0 and 1 found
 */
int OpenDevices();

/* Close devices
 * parameters: none
 * returns: void
 */
void CloseDevices();

/* Read analog channel
 * parameters:
 *   CardAddress, address of device 0 or 1
 *   Channel, channel to read, 1-8
 * returns: int
 *   value of channel 0..1023
 */
int ReadAnalogChannel(int CardAddress, int Channel);

/* Read all analog channels
 * parameters:
 *   CardAddress, address of device 0 or 1
 *   Buffer, array of 5 ints to be populated with values
 * returns: void
 */
void ReadAllAnalog(int CardAddress, int *Buffer);

/* Set PWM output
 * parameters:
 *   CardAddress, address of device 0 or 1
 *   Channel, channel on card 1 or 2 
 *   Data, value to output 0..255
 *   Freq, PWM frequency 1..3
 *     1:  2929.68 Hz
 *     2: 11718.75 Hz
 *     3: 46875 Hz
 * returns: void
 */
void SetPWM(int CardAddress, int Channel, int Data, int Freq);

/* Set both PWM outputs
 * parameters:
 *   CardAddress, address of device 0 or 1
 *   Data1, Channel 1 value 0..255
 *   Data2, Channel 2 value 0..255
 * returns: void
 */
void OutputAllPWM(int CardAddress, int Data1, int Data2);

/* Set digital outputs 
 * parameters:
 *   CardAddress, address of device 0 or 1
 *   Data, bit pattern for outputs
 * returns: void
 */
void OutputAllDigital(int CardAddress, int Data);

/* Clears digital channel (set to 0)
 * parameters:
 *   CardAddress, address of device 0 or 1
 *   Channel, channel to clear 1..8
 * returns: void
 */
void ClearDigitalChannel(int CardAddress, int Channel);

/* Clears all digital channels (set to 0)
 * parameters:
 *   CardAddress, address of device 0 or 1
 * returns: void
 */
void ClearAllDigital(int CardAddress);

/* Sets digital channel (set to 1)
 * parameters:
 *   CardAddress, address of device 0 or 1
 *   Channel, channel to set 1..8
 * returns: void
 */
void SetDigitalChannel(int CardAddress, int Channel);

/* Sets all digital channels (set to 1)
 * parameters:
 *   CardAddress, address of device 0 or 1
 * returns: void
 */
void SetAllDigital(int CardAddress);

/* Reads digital channel
 * parameters:
 *   CardAddress, address of device 0 or 1
 *   Channel, channel to read 1..8
 * returns: bool
 *   value of channel true/false
 */
bool ReadDigitalChannel(int CardAddress, int Channel);

/* Reads all digital channels
 * parameters:
 *   CardAddress, address of device 0 or 1
 * returns: int
 *   bitpattern of digital channels
 */
int ReadAllDigital(int CardAddress);

/* Sets the input/output mode of digital channels
 * parameters:
 *   CardAddress, address of device 0 or 1
 *   HighNibble, mode of channels 5-8, 0 output, 1 input
 *   LowNibble, mode of channels 1-4, 0 output, 1 input
 * returns: void
 */
void InOutMode(int CardAddress, int HighNibble, int LowNibble);

/* Read pulse counter on digital 1
 * parameters:
 *   CardAddress, address of device 0 or 1
 * returns: unsigned int
 *   value of pulse counter
 */
unsigned int ReadCounter(int CardAddress);

/* Reset pulse counter on digital 1 to 0
 * parameters:
 *   CardAddress, address of device 0 or 1
 * returns: void
 */
void ResetCounter(int CardAddress);

/* Check connection status
 * parameters: none
 * returns: int
 *   0: No devices connected
 *   1: Device address 0 connected
 *   2: Device address 1 connected
 *   3: Deivces 0 and 1 connected
 */
int Connected();

/* Check firmware version for card
 * parameters:
 *   CardAddress, address of device 0 or 1
 * returns: int
 *   is 4 byte sized fields each representing
 *   one digit of the firmware version
 */
int VersionFirmware(int CardAddress);

/* Check dll version
 * parameters: none
 * returns: int
 *   is 4 byte sized fields each representing
 *   one digit of the dll version   
 */
int VersionDLL();

/* Read the values of the PWM outputs
 * parameters:
 *   CardAddress, address of device 0 or 1
 *   Buffer, array of 2 integers to be populated
 * returns: void
 */
void ReadBackPWMOut(int CardAddress, int *Buffer);

/* Read the Input/Output mode of digital pins
 * parameters:
 *   CardAddress, address of device 0 or 1
 * returns: int
 *   0: All outputs
 *   1: 1-4 input, 5-8 output
 *   2: 1-4 output, 5-8 input
 *   3: All inputs
 */
int ReadBackInOutMode(int CardAddress);

#ifdef __cplusplus
}
#endif
#endif