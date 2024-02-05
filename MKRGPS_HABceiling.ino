/*
  uBlox SAM-M8Q I2C demo for MKR GPS
   5-Feb-2024
   Copyright (C) 2022-2024 C Turvey (aka sourcer32, clive1)
   All Rights Reserved

 If this saves you several man-hours/days consider https://paypal.me/cliveone
  5-Feb-2024  sourcer32@gmail.com

//****************************************************************************
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

void set_airbourne_dynamic(void)
{
  // Setting AIRBOURNE 1G (DYNAMIC=6), 80 KM VERTICAL, 100 M/S HORIZONTAL DYNAMICS uBlox Series 7 and 8

  uint8_t ubx_cfg_nav5[] = { // CFG-NAV5 (06 24)
    0xB5,0x62,0x06,0x24,0x24,0x00, // Header/Command/Size
    0x01,0x00,0x06,0x00,0x00,0x00,0x00,0x00, // Payload data (Dynamics flag, and Dynamic setting)
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x55,0xB4 }; // Checksum (Fletcher) of preceeding packet

  ubx_send_i2c(sizeof(ubx_cfg_nav5), ubx_cfg_nav5);
}

//***************************************************************************

void setup(void)
{
  while(!Serial) // (USB) Wait for Host Side Connection

  Serial.println("MKR GPS SAM-M8Q I2C");

  Wire.begin(); // Bring up the I2C

  set_airbourne_dynamic();
}

//***************************************************************************

void loop(void)
{
  // ..
}

//***************************************************************************
