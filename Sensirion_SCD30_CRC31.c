// CRC8-31 for SCD30

// If this saves you several man-hours/days consider https://paypal.me/cliveone
//  24-Apr-2024  sourcer32@gmail.com

// Now accepting coffee  https://buymeacoffee.com/cliveone

// https://sensirion.com/media/documents/D7CEEF4A/6165372F/Sensirion_CO2_Sensors_SCD30_Interface_Description.pdf

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

uint8_t test_crc8_31(uint8_t crc, uint8_t data)
{
  int i;

  crc = crc ^ data;

  for(i=0; i<8; i++)
    if (crc & 0x80)
      crc = (crc << 1) ^ 0x31;
    else
      crc <<= 1;

  return(crc);
}

uint8_t test_crc8_31_quick(uint8_t crc, uint8_t data)
{
  static const uint8_t crctbl[] = { // 0x31 nibble table - sourcer32@gmail.com
    0x00,0x31,0x62,0x53,0xC4,0xF5,0xA6,0x97,0xB9,0x88,0xDB,0xEA,0x7D,0x4C,0x1F,0x2E };

  crc = crc ^ data;

  crc = (crc << 4) ^ crctbl[crc >> 4]; // Process byte 4-bits at a time
  crc = (crc << 4) ^ crctbl[crc >> 4];

  return(crc);
}

int main(int argc, char **argv)
{
  uint8_t crc;

  crc = 0xFF; // pattern BE EF [92]

  crc = test_crc8_31(crc, 0xBE);
  crc = test_crc8_31(crc, 0xEF);

  printf("%02X == 92 ??\n", crc);

  crc = 0xFF; // pattern BE EF [92]

  crc = test_crc8_31_quick(crc, 0xBE);
  crc = test_crc8_31_quick(crc, 0xEF);

  printf("%02X == 92 ??\n", crc);

  return(1);
}


