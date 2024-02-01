/*

  RPi Pico uBlox I2C Simple NMEA Processing Exercise
   31-Jan-2024
   Copyright (C) 2018-2024 C Turvey (aka sourcer32, clive1)
   All Rights Reserved

  If this saves you several man-hours/days consider https://paypal.me/cliveone
   31-Jan-2024  sourcer32@gmail.com

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

int Verbose = 0; // Diagnostic Output

//****************************************************************************

#define FIELD_MAX 20

void ProcessNMEALine(char *s)
{
  char str[80];
  char *field[FIELD_MAX];
  int f;
  int i;
  char c;
  int x;

  if (Verbose)
  {
    Serial.println("DecodeNMEA");
    Serial.println(s);
  }

  // Check the line synchronization

  if (s[0] != '$')
    return;

  // Verify the line checksum integrity

  c = 0; // checksum

  i = 1; // Xor bytes between $ and *, but not including those bytes

  while((s[i] != 0) && (s[i] != '*'))
    c ^= s[i++];

  if (s[i] == 0)
    return;

  sscanf(&s[i + 1],"%x",&x); // Checksum byte - Note sscanf needs this to be an int, rather than a single byte

  if (c != (char)(x & 0xFF)) // Leave if checksum fails
    return;

  // Parse out fields on , and *

  f = 0;

  while(1)
  {
    field[f++] = s;

    while((*s != 0) && (*s != ',') && (*s != '*') && (*s != 0x0D) && (*s != 0x0A))
      s++;

    if ((*s == 0) || (*s == '*') || (*s == 0x0D) || (*s == 0x0A) || (f == (FIELD_MAX - 1)))
    {
      *s = 0;
      field[f] = NULL;
      break;
    }

    *s++ = 0;
  }

  if (Verbose)
  {
    sprintf(str, "Fields %02d",f);
    Serial.println(str);

    for(i=0; i<f; i++)
    {
      sprintf(str, "#%02d : %s",i,field[i]);
      Serial.println(str);
    }
  }

  // Process a couple of NMEA sentences for illustration

  if (strncmp(field[0],"$G",2) == 0) // Allow for different Talker ID's ie $GP, $GN, $GL, etc
  {
    if ((strcmp(&field[0][3],"GLL") == 0) && (f > 6)) // Geographic Position, Latitude, Longitude and Time
    {
      double lat, lon;
      int lat_deg, lon_deg;
      double lat_min, lon_min;
      double fix_time;
      int fix_hour, fix_minute;
      double fix_second;
      char lat_hemi, lon_hemi, valid;

      // Field 1 Latitude DDMM.mmmmmm
      // Field 2 Lat Hemi N/S
      // Field 3 Longitude DDMMM.mmmmm
      // Field 4 Lon Hemi E/W
      // Field 5 UTC Time HHMMSS.SSS
      // Field 6 Fix A=Valid, V=Not Valid
      // Field 7 Mode A=Autonomous, D=Differential, E=Estimated, N=Not Valid [Optional] - Simulator Does not report this

      lat = atof(field[1]);

      lat_hemi = field[2][0];

      lon = atof(field[3]);

      lon_hemi = field[4][0];

      fix_time = atof(field[5]);

      valid = field[6][0];

      if (valid == 'A')
      {
        // Extract Time-of-Fix

        fix_minute = (int)fix_time / 100;

        fix_second = fix_time - (fix_minute * 100);

        fix_hour = fix_minute / 100;

        fix_minute = fix_minute % 100;

        // Process Latitude DDMM.mmmmm

        lat_deg = (int)lat / 100; // Decompose NMEA form ASCII into DEGREES and MINUTES

        lat_min = lat - (lat_deg * 100);

        lat = (double)lat_deg + (lat_min / 60.0); // Computed Latitude in DECIMAL DEGREES

        if (lat_hemi == 'S')
        {
          lat_deg = -lat_deg;
          lat = -lat;
        }

        // Process Longitude DDDMM.mmmmm

        lon_deg = (int)lon / 100; // Decompose NMEA form ASCII into DEGREES and MINUTES

        lon_min = lon - (lon_deg * 100);

        lon = (double)lon_deg + (lon_min / 60.0); // Computed Longitude in DECIMAL DEGREES

        if (lon_hemi == 'W')
        {
          lon_deg = -lon_deg;
          lon = -lon;
        }

        sprintf(str, "GLL: %4d %9.6lf %4d %9.6lf [%+14.10lf %+14.10lf] @ %02d:%02d:%06.3lf",
          lat_deg, lat_min, lon_deg, lon_min, lat, lon, fix_hour, fix_minute, fix_second );
        Serial.println(str);
      }
      else
        Serial.println("Invalid Fix");
    }
    else if ((strcmp(&field[0][3],"GSA") == 0) && (f > 17)) // GPS DOP and Active Satellites
    {
      char mode, fix;
      double pdop, hdop, vdop;
      int i, sv;

      // Field 1 A=Automatic(3D/2D), M=Manual
      // Field 2 Fix 1=No Fix, 2=2D, 3=3D
      // Field 3 SV List#1
      // Field 14 SV List#12
      // Field 15 PDOP
      // Field 16 HDOP
      // Field 17 VDOP

      mode = field[1][0];
      fix = field[2][0];

      pdop = atof(field[15]); // Position Dilution of precision (PDOP)
      hdop = atof(field[16]); // Horizontal Dilution of precision (HDOP)
      vdop = atof(field[17]); // Vertical Dilution of precision (VDOP)

      switch(mode)
      {
        case 'A' : Serial.println("Mode : Automatic"); break;
        case 'M' : Serial.println("Mode : Manual"); break;
        default : Serial.println("Mode : Unknown");
      }

      switch(fix)
      {
        case '1' : Serial.println("Fix : Not Available"); break;
        case '2' : Serial.println("Fix : 2D"); break;
        case '3' : Serial.println("Fix : 3D"); break;
        default : Serial.println("Fix : Unknown");
      }

      Serial.print("SV :");

      for(i=0; i<12; i++)
      {
        if (field[3+i][0])
        {
          sscanf(field[3+i],"%d",&sv);
          sprintf(str," %3d", sv);
          Serial.print(str);
        }
      }

      Serial.println();

      sprintf(str, "PDOP : %5.2lf, HDOP : %5.2lf, VDOP : %5.2lf", pdop, hdop, vdop);
      Serial.println(str);
    }
    else if ((strcmp(&field[0][3],"GGA") == 0) && (f > 14))
    {
      double lat, lon, alt, msl;
      int lat_deg, lon_deg;
      double lat_min, lon_min;
      double fix_time;
      int fix_hour, fix_minute;
      double fix_second;
      char lat_hemi, lon_hemi;
      int valid;
      double hdop;
      int sv;

      // Field  1 UTC Time HHMMSS.SSS
      // Field  2 Latitude
      // Field  3 Lat Hemi
      // Field  4 Longitude
      // Field  5 Lon Hemi
      // Field  6 Position Fix Indicator - 0=Fix Not available, 1=GPS SPS, 2=GPS SPS DIFF, 3=GPS PPS,
      //              4=RTK, 5=Float RTK, 6=Estimate/Dead Reckoning, 7=Manual, 8=Simulator
      // Field  7 Satellites Used
      // Field  8 HDOP
      // Field  9 MSL Altitude
      // Field 10 Units M
      // Field 11 Geoid Separation
      // Field 12 Units M
      // Field 13 Age of Differential (NULL when not used)
      // Field 14 Differential Reference Station ID

      lat = atof(field[2]);

      lat_hemi = field[3][0];

      lon = atof(field[4]);

      lon_hemi = field[5][0];

      msl = atof(field[9]);

      alt = atof(field[11]);

      fix_time = atof(field[1]);

      if (sscanf(field[6],"%d",&valid) != 1)// 0=Fix Not available, 1=GPS SPS, 2=GPS SPS DIFF
        valid = 0;

      sscanf(field[7],"%d",&sv);    // Satellites used (or in view, depends on receiver)

      hdop = atof(field[8]); // Horizontal Dilution of precision (HDOP)

      if (valid != 0)
      {
        lat_deg = (int)lat / 100;

        lat_min = lat - (lat_deg * 100);

        lat = (double)lat_deg + (lat_min / 60.0);

        if (lat_hemi == 'S')
          lat = -lat;


        lon_deg = (int)lon / 100;

        lon_min = lon - (lon_deg * 100);

        lon = (double)lon_deg + (lon_min / 60.0);

        if (lon_hemi == 'W')
          lon = -lon;

        alt += msl; // Convert to height above datum

        fix_minute = (int)fix_time / 100;

        fix_second = fix_time - (fix_minute * 100);

        fix_hour = fix_minute / 100;

        fix_minute = fix_minute % 100;

        sprintf(str, "GGA: %+14.10lf %+14.10lf %8.2lf @ %02d:%02d:%06.3lf",
            lat, lon, alt,
            fix_hour, fix_minute, fix_second);
        Serial.println(str);

        if (valid == 1) Serial.println("GNSS");
        if (valid == 2) Serial.println("Differential GNSS");
        if (valid == 4) Serial.println("RTK Fixed");
        if (valid == 5) Serial.println("RTK Float");
      }
    }
  }
}

//****************************************************************************

#define LINEMAX 200 // Maximal allowed/expected line length

void ProcessNMEAStream(uint8_t *Buffer, int Size)
{
  static char rx_buffer[LINEMAX + 1];   // Local holding buffer to build line, with space for terminating NUL
  static int rx_index = 0;

  while(Size--) // Process available data
  {
    char rx = (char)*Buffer++;

    if ((rx == '\r') || (rx == '\n')) // Is this an end-of-line condition, either will suffice?
    {
      if (rx_index != 0) // Line has some content
      {
        rx_buffer[rx_index] = 0; // Add terminating NUL

        ProcessNMEALine(rx_buffer);

        rx_index = 0; // Reset content pointer
      }
    }
    else
    {
      if ((rx == '$') || (rx_index == LINEMAX)) // If resync or overflows pull back to start
        rx_index = 0;

      rx_buffer[rx_index++] = rx; // Copy to buffer and increment
    }
  }
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
      ProcessNMEAStream(buffer, xferlen); // Crack NMEA Sentences
    ubx_avail -= xferlen;
    if (ubx_avail == 0) // Refresh Pending
      ubx_avail = ubx_read_available();
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

//#define USB_BAUD 38400
#define USB_BAUD 115200

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin(); // Bring up the I2C

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
  Serial.println("RPiPICO_UBLOX_I2C_NMEA Starting");

  // Send something to precipitate a response, unstall interface
  uint8_t ubx_nav_posllh_req[] = { 0xB5,0x62,0x01,0x02,0x00,0x00,0x03,0x0A }; // Zero payload request form

  ubx_send_i2c(sizeof(ubx_nav_posllh_req), ubx_nav_posllh_req);
}

//****************************************************************************
