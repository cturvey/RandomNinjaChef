/*

  Simple NMEA Processing Exercise - sourcer32@gmail.com
    Copyright (C) 2018-2021, All Rights Reserved

  Contribute via https://paypal.me/cliveone

  Arduino Nano IoT 33 board, proxy for RP2040

  5V (VBUS LINK MADE) to ARDUSIMPLE.5V_IN
  GND to ARDUSIMPLE.GND
  3V3 to ARDUSIMPLE.IOREF
  RX0 to ARDUSIMPLE.TX1

*/

//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void ProcessStream(uint8_t *Buffer, int Size)
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

void loop()
{
  int len = Serial1.available();
  while(len > 0) // Do blocks of bytes for efficiency
  {
    uint8_t buffer[128];
    int xferlen = len;
    if (len > sizeof(buffer)) xferlen = sizeof(buffer);
    Serial1.readBytes(buffer, xferlen);

    if (Verbose) Serial.write(buffer, xferlen);

    ProcessStream(buffer, xferlen);

    len -=xferlen;
  }
}

//****************************************************************************

void setup()
{
  Serial1.begin(38400); // ZED-F9P UART1 on this port
  while(!Serial1);

  Serial.begin(115200); // USB to PC
  while(!Serial);

  Serial.println();
  Serial.println("NANOIOT33_ArduSimpleRTK2B_NMEA Starting");
}

//****************************************************************************

