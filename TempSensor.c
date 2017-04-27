/* ========================================
*
* Copyright YOUR COMPANY, THE YEAR
* All Rights Reserved
* UNPUBLISHED, LICENSED SOFTWARE.
*
* CONFIDENTIAL AND PROPRIETARY INFORMATION
* WHICH IS THE PROPERTY OF your company.
*
* ========================================
*/

/* [] END OF FILE */

#include "TempSensor.h"

uint8 OneWire_Reset(uint8 bus) //Pulldown the required "Bus Reset". 
{
	// senden des low signals zur initialisierung
	OneWire_ControlReg_SEL_Write(bus);
	OneWire_ControlReg_DRV_Write(LOW);
	CyDelayUs(ResetPulse); // 480 - 960 us low signal
	OneWire_ControlReg_DRV_Write(HIGH);

	// warten auf input
	CyDelayUs(65);  // 15 - 60 us verarbeitungszeit

	uint8 BusPin = OneWire_StatusReg_BUS_Read(); // low signal wenn vorhanden, sonst high
	CyDelayUs(415); // 60 - 240 us presence pulse

	return((~BusPin) & 255u); // rückgabe 1 falls present, 0 wenn nicht
}

void W_Bit(uint8 payload) // Write a single bit to the 1-WIRE bus. 
{
	OneWire_ControlReg_DRV_Write(LOW); // Drive LOW enabled outputs
	CyDelayUs(5);

	if (payload) { // high signal
		OneWire_ControlReg_DRV_Write(HIGH);
		CyDelayUs(55);
	}
	else { // low signal
		CyDelayUs(55);
		OneWire_ControlReg_DRV_Write(HIGH);
	}

	CyDelayUs(1);
}


void OneWire_WriteByte(uint8 payload, uint8 bus) //Write a byte to the slave.
{
	OneWire_ControlReg_SEL_Write(bus);

	uint8 BitPayload, i;
	for (i = 0; i < 8; i++)
	{
		BitPayload = (payload >> i) & 0x01;
		W_Bit(BitPayload);
	}
}

uint8 R_Bit(uint8 bus) //  Read a single bit off the 1-WIRE bus
{
	// 1 us low signal um bit zu holen
	OneWire_ControlReg_DRV_Write(LOW);
	OneWire_ControlReg_SEL_Write(bus);
	CyDelayUs(1);
	OneWire_ControlReg_DRV_Write(HIGH);

	// 5 us warten
	CyDelayUs(5);

	// signal auslesen und 60 us - 5 us = 55 us warten
	uint8 result = OneWire_StatusReg_BUS_Read();
	CyDelayUs(55);

	return (result & bus);
}

int OneWire_ReadByte(uint8 bus)
{
	uint8 bits = 8;
	int value = 1 << (bits - 1);
	int data = 0;
	volatile int timeout = 0;
	while (bits > 0)
	{
		data >>= 1;
		if (R_Bit(bus))
		{
			data |= value;
		}
		--bits;
		CyDelayUs(1);
		if (++timeout>1000) {
			return 255;
		}
	}

	return data;
}

void Write_TH(uint8 value, uint8 bus) {
	OneWire_Reset(bus);
	OneWire_WriteByte(CMD_Write_TH, bus);
	OneWire_WriteByte(value, bus);
}

void Write_TL(uint8 value, uint8 bus) {
	OneWire_Reset(bus);
	OneWire_WriteByte(CMD_Write_TL, bus);
	OneWire_WriteByte(value, bus);
}

uint8 Read_TH(uint8 bus) {
	OneWire_Reset(bus);
	OneWire_WriteByte(CMD_Read_TH, bus);
	return OneWire_ReadByte(bus);
}

uint8 Read_TL(uint8 bus) {
	OneWire_Reset(bus);
	OneWire_WriteByte(CMD_Read_TL, bus);
	return OneWire_ReadByte(bus);
}

void Start_Convert(uint8 bus) {
	OneWire_Reset(bus);
	OneWire_WriteByte(CMD_Start_Convert, bus);
}

void Stop_Convert(uint8 bus) {
	OneWire_Reset(bus);
	OneWire_WriteByte(CMD_Stop_Convert, bus);
}

void Write_Status(uint8 value, uint8 bus) {
	OneWire_Reset(bus);
	OneWire_WriteByte(CMD_Write_Status, bus);
	OneWire_WriteByte(value, bus);
}

uint8 Read_Status(uint8 bus) {
	OneWire_Reset(bus);
	OneWire_WriteByte(CMD_Read_Status, bus);
	return OneWire_ReadByte(bus);
}

uint8 Read_Temperature(uint8 bus, uint8 force) {
	if (!force) {
		volatile uint8 status;
		volatile int timeout = 0;
		do
		{

			status = Read_Status(bus);

			if (++timeout>1000) {
				return 255;
			}
			CyDelayUs(1);
		} while (!(status & 0x80)); // first bit -> finish conversion
	}

	OneWire_Reset(bus);
	OneWire_WriteByte(CMD_Read_Temperature, bus);
	return OneWire_ReadByte(bus);
}

