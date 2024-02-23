/*

  RPi Pico uBlox UART Simple UBX Processing Exercise
   22-Feb-2024
   Copyright (C) 2018-2024 C Turvey (aka sourcer32, clive1)
   All Rights Reserved

  If this saves you several man-hours/days consider https://paypal.me/cliveone
   22-Feb-2024  sourcer32@gmail.com

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

//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

  Serial.println("Send UBX-CFG-NAV5 Setting Airbourne 1G");
  
  Serial1.write(ubx_cfg_nav5, sizeof(ubx_cfg_nav5));
}

//****************************************************************************

#define BEAT 200

//****************************************************************************

void loop(void)
{
  static uint32_t start = millis();

  static uint8_t response[6 + 36 + 2]; // La lunghezza del messaggio di risposta NAV5 � Header+Payload+Sums
  static uint8_t i = 0; // static so it accumulates over loop() calls

  // Attendi la risposta del modulo GPS
  if (Serial1.available()) {
    while (Serial1.available()) {
      response[i++] = Serial1.read();

#ifdef DEBUG
if (i > 1)
{
  char string[32];
  sprintf(string,"%02X ",response[i-1]);
  Serial.println(string);
}
#endif

  if ((i == 1) && (response[0] != 0xB5)) i = 0; // Resync, want 0xB5 as first byte
  if ((i == 2) && (response[1] != 0x62)) i = 0; // Resync, want 0xB5,0x62 as second pattern

  if ((i == 6) && ((response[0] != 0xB5) || // Patterns to recognize
                   (response[1] != 0x62) ||
                   (response[2] != 0x06) ||
                   (response[3] != 0x24) ||
                   (response[4] != 0x24) || // 36-byte payload
                   (response[5] != 0x00)) )  i = 0; // Resync

      if (i >= sizeof(response)) break; // Prevenire overflow dell'array
    } // while

    // Controlla se la risposta � un messaggio NAV5
    if (i == sizeof(response) && response[0] == 0xB5 && response[1] == 0x62 && response[2] == 0x06 && response[3] == 0x24) {
      // Estrai il modello dinamico dalla risposta
      uint8_t dynModel = response[6 + 2]; // +2 Byte of Payload, Payload starts at byte 6 of the UBX Packet
      Serial.print("Modello dinamico: ");
      switch(dynModel) {
        case 0: Serial.println("Portable"); break;
        case 2: Serial.println("Stationary"); break;
        case 3: Serial.println("Pedestrian"); break;
        case 4: Serial.println("Automotive"); break;
        case 5: Serial.println("Sea"); break;
        case 6: Serial.println("Airborne <1g acceleration"); break;
        case 7: Serial.println("Airborne <2g acceleration"); break;
        case 8: Serial.println("Airborne <4g acceleration"); break;
        case 9: Serial.println("Wrist-worn watch"); break;
        case 10: Serial.println("Motorbike"); break;
        case 11: Serial.println("Robotic lawn mower"); break;
        case 12: Serial.println("Electric kick scooter"); break;
        default: Serial.println("Sconosciuto");
      } // switch
      i = 0; // Restart
    } // if
  } // if available

  if ((millis() - start) >= BEAT) // HeatBeat - GP25
  {
    static uint32_t led = 0;
    digitalWrite(LED_BUILTIN, (led & 1) ? HIGH : LOW);
    led++;
    start += BEAT;
  }

  if (Serial.available()) // Keyboard Input
  {
    char x = Serial.read();
    if ((x == 'D') || (x =='d'))
    {
      Serial.println("Send UBX-CFG-NAV5 Query");
      uint8_t ubx_cfg_nav5_req[] = { 0xB5,0x62,0x06,0x24,0x00,0x00,0x2A,0x84 }; // Zero payload request form
      Serial1.write(ubx_cfg_nav5_req, sizeof(ubx_cfg_nav5_req));
    }
    if ((x == 'A') || (x =='a'))
      set_airbourne_dynamic();
  }
} // loop

//****************************************************************************

//#define USB_BAUD 38400
#define USB_BAUD 115200

#define GNSS_BAUD 9600

//****************************************************************************

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);

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
  Serial.println("RPiPICO_UBLOX_UART_UBX_NAV5 Starting");

  Serial1.begin(GNSS_BAUD); // GPS connected to D0/D1 GP01

  while(Serial1.available()) // Flush
    Serial1.read();
    
  uint8_t ubx_cfg_nav5_req[] = { 0xB5,0x62,0x06,0x24,0x00,0x00,0x2A,0x84 }; // Zero payload request form
  Serial1.write(ubx_cfg_nav5_req, sizeof(ubx_cfg_nav5_req));
}

//****************************************************************************
