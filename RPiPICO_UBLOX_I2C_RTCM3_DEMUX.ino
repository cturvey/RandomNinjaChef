/*

  RPi Pico uBlox I2C RTCM3 Demux / Dumper
   24-Apr-2025
   Copyright (C) 2025 C Turvey (aka sourcer32, clive1)
   All Rights Reserved

 If this saves you several man-hours/days consider https://paypal.me/cliveone
  24-Apr-2025  sourcer32@gmail.com

PICO

GP25 LED

 1  D0 GP00 TX  40  VBUS (5V)
 2  D1 GP01 RX  39  VSYS
 3  GND         38  GND
 4  GP02        37  3V3_EN
 5  GP03        36  3V3_OUT
 6  GP04 SDA    35  ADC_VREF
 7  GP05 SCL    34  GP28 ADC2
 8  GND         33  GND
 9  GP06        32  GP27 ADC1
10  GP07        31  GP26 ADC0
11  GP08        30  RUN
12  GP09        29  GP22
13  GND         28  GND
14  GP10        27  GP21
15  GP11        26  GP20
16  GP12        25  GP19
17  GP13        24  GP18
18  GND         23  GND
19  GP14        22  GP17
20  GP15        21  GP16

Serial1 GP00/GP01
Wire    GP04/GP05

C:\Users\xxx\AppData\Local\Arduino15\packages\arduino\hardware\mbed_rp2040\4.0.8\variants\RASPBERRY_PI_PICO\pins_arduino.h

 Would power uBlox / SparkFun boards via VBUS (5V) as appropriate.
 Common ground
 Pin 6 (GP04) SDA
 Pin 7 (GP05) SCL

*/

#include <Wire.h>  // Arduino standard I2C/Two-Wire Library

//***************************************************************************
// Quick I2C
//***************************************************************************

#define UBX_I2C_ADDR 0x42

// Assumptions, you'll send at least 3-bytes, ideally a whole packet
//  min packet size with nothing would be 6 bytes

#define MAX_I2C_UBX_SEND 32 // This is some mythical limitation of Arduino Library

int ubx_send_i2c(size_t size, const uint8_t *buffer) // sourcer32@gmail.com
{
  while(size)
  {
    size_t length = size;

    // A method to get at least 5 bytes left to send in final packet
    if (length > MAX_I2C_UBX_SEND) length = MAX_I2C_UBX_SEND - 5;

    // Begin transmission to the I2C slave device
    Wire.beginTransmission(UBX_I2C_ADDR);

    // Queue data array for transmission to the I2C device
    if (Wire.write(buffer, length) != length) // Was it accepted?
    {
      Wire.endTransmission(true); // Send and Close the I2C interface.
      return(0); // Failed
    }

    // Transmit the bytes and a stop message to release the I2C bus.
    if (Wire.endTransmission(true))
      return(0); // Failed

    size -= length;
    buffer += length;
  }

  return(1); // Succeed
}

//***************************************************************************

int ubx_read_i2c(uint8_t reg, size_t size, uint8_t *buffer)
{
  // Begin transmission to the I2C slave device
  Wire.beginTransmission(UBX_I2C_ADDR);
  Wire.write(&reg, sizeof(reg));
  if (Wire.endTransmission(false))
    return(0); // Failed
  Wire.requestFrom(UBX_I2C_ADDR, size); // Request bytes
  while(Wire.available() < size) {}; // Wait for delivery
  Wire.readBytes(buffer, size);
  return(1); // Succeed
}

//***************************************************************************

int ubx_write_i2c_byte(uint8_t reg, uint8_t byte)
{
  // Begin transmission to the I2C slave device
  Wire.beginTransmission(UBX_I2C_ADDR);
  Wire.write(&reg, sizeof(reg));
  Wire.write(&byte, sizeof(byte));
  if (Wire.endTransmission(true))
    return(0); // Failed
  return(1); // Succeed
}

//***************************************************************************

int ubx_write(uint8_t *buffer, size_t size)
{
  if (size >= 3)
    return(ubx_send_i2c(size, buffer));
  else
  {
    while(size--)
    {
      if (!ubx_write_i2c_byte(0xFF, *buffer++)) return(0);
    }
    return(1);
  }
}

//***************************************************************************

int ubx_read_available(void)
{
  uint8_t buffer[4];
  int count = 0;
  if (ubx_read_i2c(0xFD, 2, buffer)) // Read a byte count from registers 0xFD/0xFE, 2-bytes
    count = ((int)buffer[0] << 8) + (int)buffer[1]; // MSB first
  return(count);
}

//****************************************************************************

void DumpData(size_t size, uint8_t *buffer)
{
  char str[64];

  while(size)
  {
    char *s = str;
    size_t i = size;
    if (i > 16) i = 16;
    size -= i;
    while(i--)
      s += sprintf(s, " %02X", *buffer++);
    Serial.println(str);
  }
}
//****************************************************************************
// RTCM3 Process / Pull-Down
//****************************************************************************

uint32_t Crc24Quick(uint32_t Crc, uint32_t Size, uint8_t *Buffer)
{
  static const uint32_t crctab[] = { // Nibble Table
    0x00000000,0x01864CFB,0x038AD50D,0x020C99F6,0x0793E6E1,0x0615AA1A,0x041933EC,0x059F7F17,
    0x0FA18139,0x0E27CDC2,0x0C2B5434,0x0DAD18CF,0x083267D8,0x09B42B23,0x0BB8B2D5,0x0A3EFE2E };

  while(Size--)
  {
    Crc ^= (uint32_t)*Buffer++ << 16;

    Crc = (Crc << 4) ^ crctab[(Crc >> 20) & 0x0F]; // Consume Byte, 4-bits at a time
    Crc = (Crc << 4) ^ crctab[(Crc >> 20) & 0x0F];
  }

  return(Crc & 0xFFFFFF);
}

//****************************************************************************

uint32_t DumpRTCM3(uint32_t Size, uint8_t *Buffer)
{
  if (Size < 3)
  {
    if (Size && (Buffer[0] != 0xD3)) // Dump/Sync
      return(1); // Skip / Advance
    else
      return(0); // Insufficient to determine
  }

  if (Buffer[0] != 0xD3) // Check Preamble, Dump/Sync
  {
    uint32_t i = 0;
    while(Size-- && (Buffer[i] != 0xD3)) i++;
    return(i); // Skip / Advance
  }

  if ((Buffer[1] & 0xFC) != 0x00) // Check second byte consistency, 6 high order bits zero
    return(1); // Dump/Sync

  if ((Size >= 6) && (Buffer[0] == 0xD3) && ((Buffer[1] & 0xFC) == 0x00)) // Check Header
  {
    uint32_t RecordSize = (((uint32_t)Buffer[1] & 0x03) << 8) + ((uint32_t)Buffer[2] << 0) + 6; // Header/Payload/CRC
    uint16_t Msg = ((uint16_t)Buffer[3] << 4) + ((uint16_t)Buffer[4] >> 4);

    if (Size >= RecordSize) // Check Header/Payload/CRC
    {
      char String[64];

      sprintf(String, "RTCM3 %4d %4d", Msg, RecordSize);
      Serial.println(String);

      if (Crc24Quick(0x000000, RecordSize, Buffer) != 0) // Invalid CRC24
      {
        Serial.println("Bad CRC");
        return(1); // Dump/Sync
      }
      else
      {
        // Whole intact RTCM3 packet in buffer
        DumpData(RecordSize, Buffer);

        return(RecordSize);
      }
    }
    else
      return(0); // Not Enough to check
  }

  return(0); // Insufficient
}

//****************************************************************************

void ProcessRTCM3(uint32_t Size, uint8_t *Buffer)
{
  while(Size)
  {
    static uint8_t SerialBuffer[1024 + 6]; // Holding buffer
    static uint32_t Pending = 0; // Utilization of buffer

    uint32_t Take = min(Size, sizeof(SerialBuffer) - Pending); // Available Space
    uint32_t Offset = 0;

    if (Take) // Draw down passed in buffer
    {
      memcpy(SerialBuffer + Pending, Buffer, Take); // Fill

      Size    -= Take;
      Buffer  += Take;
      Pending += Take;
    }

    while(Pending >= 3) // Enough to entertain
    {
      uint32_t PacketSize = DumpRTCM3(Pending, SerialBuffer+Offset); // Consume

      Pending -= min(PacketSize, Pending);
      Offset  += PacketSize;

      if (PacketSize == 0) break; // Insufficient
    } // while()

    if (Pending && Offset) // Any data not at front of buffer
      memcpy(SerialBuffer, SerialBuffer+Offset, Pending); // Pull-Down
  } // while()
}

//****************************************************************************

#define BEAT 200

void loop(void)
{
  static uint32_t start = millis();
  static int ubx_avail = 0;
  uint8_t buffer[64];

  if (ubx_avail > 0)
  {
    int xferlen = ubx_avail;
    if (xferlen > sizeof(buffer)) xferlen = sizeof(buffer);
    if (ubx_read_i2c(0xFF, xferlen, buffer)) // Read block from register 0xFF
      ProcessRTCM3((uint32_t)xferlen, buffer); // Digest Stream
    ubx_avail -= xferlen;
  }

  if ((millis() - start) >= BEAT) // HeatBeat - GP25
  {
    static uint32_t led = 0;
    digitalWrite(LED_BUILTIN, (led & 1) ? HIGH : LOW);
    led++;
    start += BEAT;

    if (ubx_avail == 0) // Periodic so as not to overwhelm with I2C traffic/queries
      ubx_avail = ubx_read_available();
  }
}

//****************************************************************************

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin(); // Bring up the I2C

//#define USB_BAUD 38400
#define USB_BAUD 115200

  Serial.begin(USB_BAUD); // USB to PC
  while(!Serial) // Wait for attachment
  {
    static uint32_t start = millis();
    if ((millis() - start) >= 100)  // Fast HeatBeat - GREEN LED GP25
    {
      static uint32_t led = 0;
      digitalWrite(LED_BUILTIN, (led & 1) ? HIGH : LOW);
      led++;
      start += 100;
    }
  }

  Serial.println();
  Serial.println("RPiPICO_UBLOX_I2C_RTCM3_DEMUX Starting");

  // Send something to precipitate a response, unstall interface
  uint8_t ubx_nav_posllh_req[] = { 0xB5,0x62,0x01,0x02,0x00,0x00,0x03,0x0A }; // Zero payload request form

  ubx_send_i2c(sizeof(ubx_nav_posllh_req), ubx_nav_posllh_req);
}

//****************************************************************************
