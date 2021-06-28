// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p00008HKDWQCA5/decoding-rtcm3-message
// https://portal.u-blox.com/s/question/0D52p00009IthhBCAR/ublox-rtcm-wrapper-specification
// https://portal.u-blox.com/s/question/0D52p00008HKD3FCAX/rtcm-msm4-vs-msm7
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned long uint32_t;

//****************************************************************************

uint32_t Crc24Quick(uint32_t Crc, uint32_t Size, uint8_t *Buffer) // sourcer32@gmail.com
{
  static const uint32_t crctab[] = { // Nibble lookup for Qualcomm CRC-24Q
    0x00000000,0x01864CFB,0x038AD50D,0x020C99F6,0x0793E6E1,0x0615AA1A,0x041933EC,0x059F7F17,
    0x0FA18139,0x0E27CDC2,0x0C2B5434,0x0DAD18CF,0x083267D8,0x09B42B23,0x0BB8B2D5,0x0A3EFE2E };

  while(Size--)
  {
    Crc ^= (uint32_t)*Buffer++ << 16; // Apply byte
    // Process 8-bits, 4 at a time, or 2 rounds
    Crc = (Crc << 4) ^ crctab[(Crc >> 20) & 0x0F];
    Crc = (Crc << 4) ^ crctab[(Crc >> 20) & 0x0F];
  }

  return(Crc & 0xFFFFFF); // Mask to 24-bit, as above optimized for 32-bit
}

//****************************************************************************

int main(int argc, char **argv)
{
  uint8_t rtcm3testpattern[] =
  { 0xD3,0x00,0x13,0x3E,0xD7,0xD3,0x02,0x02,0x98,0x0E,0xDE,
    0xEF,0x34,0xB4,0xBD,0x62,0xAC,0x09,0x41,0x98,0x6F,0x33,
    0x36,0x0B,0x98 };

  // The CRC across the whole packet should sum to zero (remainder)
  if (Crc24Quick(0x000000, sizeof(rtcm3testpattern), rtcm3testpattern) == 0)
    puts("Pass"); else puts("Fail");

  return(0);
}

//****************************************************************************
