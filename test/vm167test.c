/* This program requires a testrig such as
 * Digital      Analog/PWM
 * 1 ----- 5   A1 -------- GND
 * 2 ----- 6   A2 -------- +5V
 * 3 ----- 7  +5V -R-A3-R- GND
 * 4 ----- 8   A4 --R+R+-- PWM1
 *                   C-C
 *             GND---+-+	
 *
 *             A5 --R+R+-- PWM2
 *                   C-C
 *             GND---+-+
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "VM167.h"

int test_openDevices();
void test_readWriteDigital(int card);
void test_readAnalog(int card);
void test_PWM(int card);
 
int main()
{
	int devices;
	devices = OpenDevices();
	if (devices == 0)
	{
		printf("Driver error condition, exiting\n");
		fflush(stdout);
		return -1;
	}
	else if (devices == -1)
	{
		printf("No devices found on system, exiting\n");
		fflush(stdout);
		return -1;
	}
	int ver = VersionDLL();
	printf("DLL Version: %d.%d.%d.%d\n", (ver>>24), (ver>>16)&0xFF, (ver>>8)&0xFF, ver&0xFF);
	int amount = devices == 3 ? 2 : 1;
	printf("Opened %d device(s)\n", amount);
	
	if(devices&1)
	{
		int fw = VersionFirmware(0);
		printf("Card0 FW: %d.%d.%d.%d\n", (fw>>24), (fw>>16)&0xFF, (fw>>8)&0xFF, fw&0xFF);
	}
	if(devices&2)
	{
		int fw = VersionFirmware(1);
		printf("Card1 FW: %d.%d.%d.%d\n", (fw>>24), (fw>>16)&0xFF, (fw>>8)&0xFF, fw&0xFF);
	}
	
	fflush(stdout);
	sleep(1);
	
	if(devices&1)
		test_readWriteDigital(0);
	
	if(devices&2)
		test_readWriteDigital(1);
	
	if(devices&1)
		test_readAnalog(0);
	
	if(devices&2)
		test_readAnalog(1);
	
	if(devices&1)
		test_PWM(0);
	
	if(devices&2)
		test_PWM(1);
	
	CloseDevices();
	
	return 0;
}
 
void test_readWriteDigital(int card)
{
	int ch;
	int val;
	printf("Testing digital read/write on card %d\n", card);
	fflush(stdout);
	InOutMode(card, 1, 0);
	for(ch = 1; ch < 5; ++ch)
	{
		printf("Ch%d->%d: ", ch, ch+4);
		fflush(stdout);
		SetDigitalChannel(card, ch);
		sleep(1);
		val =  ReadAllDigital(card);
		ClearDigitalChannel(card, ch);
		if ((val&0x0F) == (val>>4))
			printf("%X  PASS\n", val);
		else
			printf("%X  FAIL\n", val);
		fflush(stdout);
	}
	InOutMode(card, 0, 1);
	for(ch = 5; ch < 9; ++ch)
	{
		printf("Ch%d->%d: ", ch, ch-4);
		fflush(stdout);
		SetDigitalChannel(card, ch);
		sleep(1);
		val = ReadAllDigital(card);
		ClearDigitalChannel(card, ch);
		if ((val&0x0F) == (val>>4))
			printf("%X  PASS\n", val);
		else
			printf("%X  FAIL\n", val);
		fflush(stdout);
	}
}
 
void test_readAnalog(int card)
{
	
	int i;
	int val;
	int error;
	int value[] = {1023, 0, 511};
	int tolerance = 1023*0.05;
	
	printf("Testing Analog channels on card %d tolerance 5%%\n", card);
	fflush(stdout);
	
	for (i = 0; i < 3; ++i)
	{
		printf("Analog %d: %d -> ", i, value[i]);
		fflush(stdout);
		sleep(1);
		val = ReadAnalogChannel(card, i+1);
		error = abs(val - value[i]);
		if (error < tolerance)
			printf("%d  PASS (%3.1f%%)\n", val, (100*(error/1023.0)));
		else
			printf("%d  FAIL (%3.1f%%)\n", val, (100*(error/1023.0)));
		fflush(stdout);
	}
}

void test_PWM(int card)
{
	int i;
	int val;
	int error;
	int tolerance = 255*0.1;
	int value[] = {0, 127, 255};
	
	printf("Testing PWM channels on card %d tolerance 10%%\n", card);
	fflush(stdout);
	
	for (i = 0; i < 3; ++i)
	{
		printf("PWM1: %d -> ", value[i]);
		SetPWM(card, 1, value[i], 1);
		sleep(1);
		val = ReadAnalogChannel(card, 4)/4;
		error = abs(val - value[i]);
		if (error < tolerance)
			printf("%d  PASS (%3.1f%%)\n", val, (100*(error/255.0)));
		else
			printf("%d  FAIL (%3.1f%%)\n", val, (100*(error/255.0)));
		fflush(stdout);
	}
	for (i = 0; i < 3; ++i)
	{
		printf("PWM2: %d -> ", value[i]);
		SetPWM(card, 2, value[i], 1);
		sleep(1);
		val = ReadAnalogChannel(card, 5)/4;
		error = abs(val - value[i]);
		if (error < tolerance)
			printf("%d  PASS (%3.1f%%)\n", val, (100*(error/255.0)));
		else
			printf("%d  FAIL (%3.1f%%)\n", val, (100*(error/255.0)));
		fflush(stdout);
	}
	
	OutputAllPWM(card, 0, 0);
}

 
