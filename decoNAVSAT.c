//****************************************************************************
/*
  u-Blox packet cracker example
   26-Sep-2024
   Copyright (C) 2024 C Turvey (aka sourcer32, clive1)
   All Rights Reserved
*/
//****************************************************************************

// If this saves you several man-hours/days consider https://paypal.me/cliveone
//  26-Sep-2024  sourcer32@gmail.com

// Classic MSVC
//   Build via cl -Ox decoNAVSAT.c

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#if 0
#include <stdint.h>
#else
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
#endif

typedef unsigned char U1;
typedef unsigned short U2;
typedef unsigned int U4;

typedef signed char I1;
typedef signed short I2;
typedef signed int I4;

typedef unsigned char X1;
typedef unsigned short X2;
typedef unsigned int X4;

typedef float R4;
typedef double R8;

//****************************************************************************

#pragma pack(1) // byte packing, for situation where compiler wants to pack otherwise

typedef struct _NAVSATRECORD {
  U1 gnssId;
  U1 svId;
  U1 cno;
  I1 elev;
  I2 azim;
  I2 prRes;
  X4 flags;
} NAVSATRECORD;

typedef struct _NAVSAT {
  U4 iTOW;
  U1 version;
  U1 numSvs;
  U1 reserved0[2];
  NAVSATRECORD record[1];
} NAVSAT;

#pragma pack() // normal packing/alignment

//****************************************************************************

char *decoGNSS(int x)
{
  static char string[16];
  switch(x)
  {
    case 0 : return("GPS");
    case 1 : return("SBAS");
    case 2 : return("GALILEO");
    case 3 : return("BEIDOU");
    case 5 : return("QZSS");
    case 6 : return("GLONASS");
    case 7 : return("NAVIC");
    default :
     sprintf(string, "UNK[%d]", x);
     return(string);
  }
}

//****************************************************************************

void Dump0135(uint32_t Size, uint8_t *Buffer)
{
  NAVSAT *navsat = (NAVSAT *)(Buffer + 6); // Payload Buffer
  int i, j;

  printf("TOW:%9u  SVCOUNT:%d\n", navsat->iTOW, navsat->numSvs);

  j = navsat->numSvs;

  for(i=0; i<j; i++)
  {
    printf("%2d : %-8s SV:%02d %s\n",
      i,
      decoGNSS(navsat->record[i].gnssId),
      navsat->record[i].svId,
      navsat->record[i].flags & 8 ? "USED" : "");
  }

  putchar('\n');
}

//****************************************************************************

void Dump0135Pointer(uint32_t Size, uint8_t *Buffer)
{
  uint8_t *p = (Buffer + 6); // Payload Buffer
  int i, j;
  uint32_t tow;

  if (p[4] == 1) // Version One
  {
    j = p[5]; // numSvs

    tow = ((uint32_t)p[0] <<  0) + ((uint32_t)p[1] <<  8) + // iTOW
          ((uint32_t)p[2] << 16) + ((uint32_t)p[3] << 24);

    printf("TOW:%9u  SVCOUNT:%d\n", tow, j);

    p += 8; // header size

    for(i=0; i<j; i++)
    {
      printf("%2d : %-8s SV:%02d %s\n",
        i,
        decoGNSS(p[0]), // gnssId
        p[1], // svId
        p[8] & 8 ? "USED" : ""); // flags.uvUsed

      p += 12; // record size
    }
  }

  putchar('\n');
}

//****************************************************************************
/*
---UBX-----------------------------------------------------------------------
01 35 NAV-SAT         - 85 0F : 85 0F 376
NAV-SAT  492130000
TOW:492130.000  5 16:42:10.000
Satellite Count: 30   Used: 18 (GPS:9, GLO:0, GAL:7, BDS:2) RTK
GPS       2  CNO:16 Elev:31 Azim:132 Res:-20.3      QI:4
GPS       7  CNO:19 Elev:86 Azim:192 Res:-14.8 Used QI:4
GPS       8  CNO:32 Elev:52 Azim: 54 Res:  0.3 Used QI:7 Code+Carr
GPS       9  CNO:16 Elev:18 Azim:199 Res:-11.0 Used QI:4
GPS      13  CNO:22 Elev:15 Azim:320 Res:-10.2 Used QI:4
GPS      14  CNO:28 Elev:37 Azim:279 Res: -0.4 Used QI:7 Code+Carr
GPS      17  CNO: 8 Elev:15 Azim:215 Res:  9.0      QI:4
GPS      21  CNO:15 Elev:36 Azim:113 Res:  6.2 Used QI:4
GPS      22  CNO:15 Elev:18 Azim:270 Res:  3.6 Used QI:4
GPS      27  CNO:32 Elev:16 Azim: 46 Res: -1.4 Used QI:7 Code+Carr
GPS      30  CNO:27 Elev:57 Azim:305 Res:  6.8 Used QI:7 Code+Carr
SBAS    131  CNO: 0 Elev:34 Azim:219 Res:  0.0      QI:1
SBAS    133  CNO: 0 Elev:27 Azim:232 Res:  0.0      QI:1
SBAS    138  CNO: 0 Elev:38 Azim:207 Res:  0.0      QI:1
GALILEO   4  CNO:10 Elev:17 Azim:193 Res: 19.6 Used QI:4
GALILEO   5  CNO:20 Elev:25 Azim:307 Res:-13.4 Used QI:4
GALILEO   9  CNO:22 Elev:41 Azim:244 Res:  9.6 Used QI:4
GALILEO  12  CNO: 0 Elev: 4 Azim:168 Res:  0.0      QI:1
GALILEO  13  CNO:10 Elev: 1 Azim: 21 Res: 70.9      QI:4
GALILEO  14  CNO:34 Elev:31 Azim: 56 Res:  0.0      QI:7 Code+Carr
GALILEO  24  CNO: 9 Elev:31 Azim:277 Res: 11.9 Used QI:4
GALILEO  26  CNO:26 Elev:28 Azim: 65 Res:  2.4 Used QI:7 Code+Carr
GALILEO  31  CNO:35 Elev:73 Azim:342 Res: -1.4 Used QI:7 Code+Carr
GALILEO  33  CNO:17 Elev:30 Azim:121 Res:-12.8 Used QI:4
BEIDOU    8  CNO: 0                  Res:  0.0      QI:1
BEIDOU   19  CNO:18 Elev:13 Azim:276 Res: -6.2 Used QI:4
BEIDOU   20  CNO: 0 Elev: 6 Azim:231 Res:  0.0      QI:1
BEIDOU   23  CNO: 0 Elev:14 Azim:109 Res:  0.0      QI:1
BEIDOU   43  CNO:25 Elev:14 Azim: 40 Res:  9.9 Used QI:4
BEIDOU   46  CNO:10 Elev:51 Azim:307 Res: 27.8      QI:3

*/

uint8_t ubx_nav_sat_pattern[] = {
  0xB5,0x62,0x01,0x35,0x70,0x01,0xD0,0x4E,0x55,0x1D,0x01,0x1E,0x00,0x00,0x00,0x02,
  0x10,0x1F,0x84,0x00,0x35,0xFF,0x14,0x59,0x00,0x00,0x00,0x07,0x13,0x56,0xC0,0x00,
  0x6C,0xFF,0x1C,0x59,0x00,0x00,0x00,0x08,0x20,0x34,0x36,0x00,0x03,0x00,0x1F,0x59,
  0x00,0x00,0x00,0x09,0x10,0x12,0xC7,0x00,0x92,0xFF,0x1C,0x59,0x00,0x00,0x00,0x0D,
  0x16,0x0F,0x40,0x01,0x9A,0xFF,0x1C,0x59,0x00,0x00,0x00,0x0E,0x1C,0x25,0x17,0x01,
  0xFC,0xFF,0x1F,0x59,0x00,0x00,0x00,0x11,0x08,0x0F,0xD7,0x00,0x5A,0x00,0x14,0x54,
  0x00,0x00,0x00,0x15,0x0F,0x24,0x71,0x00,0x3E,0x00,0x1C,0x59,0x00,0x00,0x00,0x16,
  0x0F,0x12,0x0E,0x01,0x24,0x00,0x1C,0x59,0x00,0x00,0x00,0x1B,0x20,0x10,0x2E,0x00,
  0xF2,0xFF,0x1F,0x59,0x00,0x00,0x00,0x1E,0x1B,0x39,0x31,0x01,0x44,0x00,0x1F,0x59,
  0x00,0x00,0x01,0x83,0x00,0x22,0xDB,0x00,0x00,0x00,0x11,0x12,0x00,0x00,0x01,0x85,
  0x00,0x1B,0xE8,0x00,0x00,0x00,0x11,0x12,0x00,0x00,0x01,0x8A,0x00,0x26,0xCF,0x00,
  0x00,0x00,0x01,0x07,0x00,0x00,0x02,0x04,0x0A,0x11,0xC1,0x00,0xC4,0x00,0x1C,0x59,
  0x00,0x00,0x02,0x05,0x14,0x19,0x33,0x01,0x7A,0xFF,0x1C,0x59,0x00,0x00,0x02,0x09,
  0x16,0x29,0xF4,0x00,0x60,0x00,0x1C,0x59,0x00,0x00,0x02,0x0C,0x00,0x04,0xA8,0x00,
  0x00,0x00,0x11,0x52,0x00,0x00,0x02,0x0D,0x0A,0x01,0x15,0x00,0xC5,0x02,0x14,0x59,
  0x00,0x00,0x02,0x0E,0x22,0x1F,0x38,0x00,0x00,0x00,0x27,0x19,0x00,0x00,0x02,0x18,
  0x09,0x1F,0x15,0x01,0x77,0x00,0x1C,0x59,0x00,0x00,0x02,0x1A,0x1A,0x1C,0x41,0x00,
  0x18,0x00,0x1F,0x59,0x00,0x00,0x02,0x1F,0x23,0x49,0x56,0x01,0xF2,0xFF,0x1F,0x59,
  0x00,0x00,0x02,0x21,0x11,0x1E,0x79,0x00,0x80,0xFF,0x1C,0x54,0x00,0x00,0x03,0x08,
  0x00,0xA5,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x03,0x13,0x12,0x0D,0x14,0x01,
  0xC2,0xFF,0x1C,0x59,0x00,0x00,0x03,0x14,0x00,0x06,0xE7,0x00,0x00,0x00,0x11,0x59,
  0x00,0x00,0x03,0x17,0x00,0x0E,0x6D,0x00,0x00,0x00,0x11,0x59,0x00,0x00,0x03,0x2B,
  0x19,0x0E,0x28,0x00,0x63,0x00,0x1C,0x49,0x00,0x00,0x03,0x2E,0x0A,0x33,0x33,0x01,
  0x16,0x01,0x13,0x49,0x00,0x00,0x85,0x0F };

//****************************************************************************

int main(int argc, char **argv)
{
  Dump0135(sizeof(ubx_nav_sat_pattern), ubx_nav_sat_pattern);

  Dump0135Pointer(sizeof(ubx_nav_sat_pattern), ubx_nav_sat_pattern);

  return(0);
}

//****************************************************************************

