/**********************************************************************
 * Linux User-space driver for Velleman VM167 USB io-card
 * USB ID: 10cf:1670
 *         10cf:1671
 * 
 * Driver is based on protocol reverse engineered from Delphi source 
 * code released by Velleman.
 * It is API compatible with the Windows driver so the same examples
 * should work.
 * 
 * Author:  Robert Ahlskog <robert.ahlskog@gmail.com>
 * Version: 0.1
 **********************************************************************
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
#include <libusb.h>
#include <stdbool.h>


libusb_context* _ctx;
libusb_device_handle *device[2];
unsigned char device_buffer[2][64];

int Write(int CardAddress, int bytes);
int Read(int CardAddress);

/* Open devices return 
 * -1 No devices found
 *  0 Driver error
 *  1 Device addr 0 found
 *  2 Device addr 1 found
 *  3 Device addr 0 and 1 found
 */
int OpenDevices()
{
	int found = 0;
	// First thing we do
	libusb_init(&_ctx);
	
	// Enumerate devices
	device[0] = libusb_open_device_with_vid_pid(_ctx, 0x10cf, 0x1670);
	device[1] = libusb_open_device_with_vid_pid(_ctx, 0x10cf, 0x1671);
	
	if (device[0] != NULL)
	{
		if (libusb_claim_interface(device[0], 0) == 0)
			found |= 1;
	}
	
	if (device[1] != NULL)
	{
		if (libusb_claim_interface(device[1], 0) == 0)
			found |= 2;
	}
	
	return found == 0 ? -1 : found;
}

/* Unload and close driver */
void CloseDevices()
{
	if (device[0] != NULL)
	{
		libusb_release_interface(device[0], 0);
		libusb_close(device[0]);
	}
	
	if (device[1] != NULL)
	{
		libusb_release_interface(device[0], 0);
		libusb_close(device[1]);
	}
	
	// Last thing we do
	libusb_exit(_ctx);
}

/* Channel between 1 and 5
 * return value between 0 and 1023 */
int ReadAnalogChannel(int CardAddress, int Channel)
{
	unsigned char *buffer = device_buffer[CardAddress];
	int transf;
	
	buffer[0] = 0; //Read analog ch
	buffer[1] = Channel -1;
	Write(CardAddress, 2);

	transf = Read(CardAddress);
	if (transf)
		return buffer[1]+256*buffer[2];
	
	return 0;
}

/* Buffer shall be an array of 5 32-bit integers */
void ReadAllAnalog(int CardAddress, int *Buffer)
{
	unsigned char *buffer = device_buffer[CardAddress];
	int transf;
	
	buffer[0] = 1;	// Read all analog
	Write(CardAddress, 1);
	
	transf = Read(CardAddress);
	if (!transf)
		return;
	
	int value;
	int i;
	for (i = 0; i < 5; ++i)
	{
		value = buffer[2*i+1] + 256*buffer[2*i+2];
		*(Buffer+i) = value;
	}
}

void SetPWM(int CardAddress, int Channel, int Data, int Freq)
{
	unsigned char *buffer = device_buffer[CardAddress];
	
	if (Channel > 2) Channel = 2;
	if (Channel < 1) Channel = 1;
	if (Freq > 3) Freq = 3;
	if (Freq < 1) Freq = 1;
	
	buffer[0] = 2;	// Set PWM
	buffer[1] = Channel-1;
	buffer[2] = Data;
	buffer[3] = Freq;
	Write(CardAddress, 4);
}

void OutputAllPWM(int CardAddress, int Data1, int Data2)
{
	unsigned char *buffer = device_buffer[CardAddress];
	
	buffer[0] = 3;	// Set both PWM channels
	buffer[1] = Data1;
	buffer[2] = Data2;
	Write(CardAddress, 3);
}

void OutputAllDigital(int CardAddress, int Data)
{
	unsigned char *buffer = device_buffer[CardAddress];
	
	buffer[0] = 5;	// Write all 8 digital channels
	buffer[1] = Data;
	Write(CardAddress, 2);
}

void ClearDigitalChannel(int CardAddress, int Channel)
{
	unsigned char *buffer = device_buffer[CardAddress];
	int k;
	
	if (Channel > 8) Channel = 8;
	if (Channel < 1) Channel = 1;
	
	// Generate a bitstring that is all ones except the target channel
	k = ~(1 << (Channel-1));
	buffer[0] = 6;  // Clear Digital Channel
	buffer[1] = k;
	
	Write(CardAddress, 2);
}

void ClearAllDigital(int CardAddress)
{
	OutputAllDigital(CardAddress, 0x00);
}

void SetDigitalChannel(int CardAddress, int Channel)
{
	unsigned char *buffer = device_buffer[CardAddress];
	int k;
	
	if (Channel>8) Channel = 8;
	if (Channel<1) Channel = 1;
	
	// Generate a bitstring that is all zeros except the target channel
	k = 1 << (Channel-1);
	buffer[0] = 7;  // Set Digital Channel
	buffer[1] = k;
	
	Write(CardAddress, 2);
}

void SetAllDigital(int CardAddress)
{
	OutputAllDigital(CardAddress, 0xFF);
}

bool ReadDigitalChannel(int CardAddress, int Channel)
{
	unsigned char byte = ReadAllDigital(CardAddress);
	if (Channel > 8) Channel = 8;
	if (Channel < 1) Channel = 1;
	
	return (byte & (1 << (Channel-1)) > 0);
}

int ReadAllDigital(int CardAddress)
{
	unsigned char *buffer = device_buffer[CardAddress];
	
	buffer[0] = 4;  // Read digital channels
	Write(CardAddress, 1);
	Read(CardAddress);
	
	return buffer[1];
}

void InOutMode(int CardAddress, int HighNibble, int LowNibble)
{
	unsigned char *buffer = device_buffer[CardAddress];
	
	buffer[0] = 8;  // In_Out_Mode
	buffer[1] = LowNibble;
	buffer[2] = HighNibble;
	
	Write(CardAddress, 3);
}

unsigned int ReadCounter(int CardAddress)
{
	unsigned char *buffer = device_buffer[CardAddress];
	
	buffer[0] = 9;  // Read onboard counter
	buffer[1] = 0;  // read counter #0
	Write(CardAddress, 2);
	Read(CardAddress);
	
	return buffer[1]+256*buffer[2]+65536*buffer[3]+16777216*buffer[4];
}

void ResetCounter(int CardAddress)
{
	unsigned char *buffer = device_buffer[CardAddress];
	
	buffer[0] = 10;  // Reset onboard counter
	buffer[1] = 0;   // reset counter #0
	Write(CardAddress, 2);
}

/* Connected method, I have probably misinterpeted the function
 * and botched my implementation */
int Connected()
{
	int cards;
	unsigned char *buffer = device_buffer[0];
	
	buffer[0] = 14;	// Unknown function
	buffer[1] = 0;
	Write(0, 2);
	Read(0);
	cards = buffer[1];
	
	buffer = device_buffer[1];
	buffer[0] = 14;	// Unknown function
	buffer[1] = 1;
	Write(1, 2);
	Read(1);
	cards = cards + buffer[1];
	
	return 0;
}

int VersionFirmware(int CardAddress)
{
	unsigned char *buffer = device_buffer[CardAddress];
	
	buffer[0] = 11;	// Read version info from card
	Write(CardAddress, 1);
	Read(CardAddress);
	
	return 256*256*256*buffer[1]+256*256*buffer[2]+256*buffer[3]+buffer[4];
}

int VersionDLL()
{
	return 0x0010013;
}

void ReadBackPWMOut(int CardAddress, int *Buffer)
{
	unsigned char *buffer = device_buffer[CardAddress];

	buffer[0] = 12;	// Read PWM
	Write(CardAddress, 1);
	Read(CardAddress);
	*Buffer = buffer[1];
	++Buffer;
	*Buffer = buffer[2];
}

int ReadBackInOutMode(int CardAddress)
{
	unsigned char *buffer = device_buffer[CardAddress];
	
	buffer[0] = 13;	// Read In/Out Mode
	Write(CardAddress, 1);
	Read(CardAddress);
	return buffer[1];
}

int Write(int CardAddress, int bytes)
{
	int transf;
	int result;
	result = libusb_bulk_transfer(device[CardAddress], 0x01, device_buffer[CardAddress], bytes, &transf, 1000);
	
	if (result != 0);
		; // How embarrasing, what do we do now?

	
	return transf;
}

int Read(int CardAddress)
{
	int transf;
	int result;
	result = libusb_bulk_transfer(device[CardAddress], 0x81, device_buffer[CardAddress], 64, &transf, 1000);
	
	if (result != 0);
		; // How embarrasing, what do we do now?

	
	return transf;
}