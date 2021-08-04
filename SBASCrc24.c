// From some conversations, ramblings and postings on the u-Blox forum
//  https://portal.u-blox.com/s/question/0D52p0000BAKgARCQ1/sbas-navigation-data-has-different-number-of-words-than-in-zedf9p-description
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned long uint32_t;

//****************************************************************************

int CheckSBASCrc(uint32_t *data)
{
  static const crctbl[] = { // 4-bit table for CRC24Q  sourcer32@gmail.com
    0x000000,0x864CFB,0x8AD50D,0x0C99F6,0x93E6E1,0x15AA1A,0x1933EC,0x9F7F17,
    0xA18139,0x27CDC2,0x2B5434,0xAD18CF,0x3267D8,0xB42B23,0xB8B2D5,0x3EFE2E };

  int i;
  uint32_t crc;
  uint32_t framecrc;

  crc = 0;

  // 250-bit SBAS Frame, from 500-bits w/FEC, CRC24 over first 226-bits

  for(i=0; i<7; i++) // Whole Words (224-Bits)
  {
    crc = crc ^ ((data[i] >> 8) & 0xFFFF00); // High Order 16-bits, aligned into 24-bit crc

    crc = (crc << 4) ^ crctbl[(crc >> 20) & 0x0F]; // 16-bits, 4-bits at a time
    crc = (crc << 4) ^ crctbl[(crc >> 20) & 0x0F];
    crc = (crc << 4) ^ crctbl[(crc >> 20) & 0x0F];
    crc = (crc << 4) ^ crctbl[(crc >> 20) & 0x0F];

    crc = crc ^ ((data[i] << 8) & 0xFFFF00); // Low Order 16-bits

    crc = (crc << 4) ^ crctbl[(crc >> 20) & 0x0F]; // 16-bits, 4-bits at a time
    crc = (crc << 4) ^ crctbl[(crc >> 20) & 0x0F];
    crc = (crc << 4) ^ crctbl[(crc >> 20) & 0x0F];
    crc = (crc << 4) ^ crctbl[(crc >> 20) & 0x0F];
  }

  crc = crc ^ ((data[i] >> 8) & 0xC00000); // Last Two Bits (224, 225)

  crc = (crc << 2) ^ crctbl[(crc >> 22) & 0x03]; // 2-bits

  crc &= 0xFFFFFF; // Mask to 24-bits, as above optimized for 32-bit

  framecrc = ((data[i] >> 6) & 0xFFFFFF); // Extract bits 226..249 Frame CRC24

  printf("CRC24:%06X  FRAMECRC:%06X\n", crc, framecrc);

  return(crc == framecrc ? 1 : 0);
}

//****************************************************************************

int main(int argc, char **argv)
{
  uint32_t sbastestpattern[] = { // UBX-RXM-SFRBX gives 9 words, we use 8
    0xC60E8000,0x003FC000,0x00000000,0x07FE4000,0x00400000,
    0x3FDFB97B,0xB957A3B9,0x46EFAE14,0xD8000000 };

  if (CheckSBASCrc(sbastestpattern))
    puts("Pass"); else puts("Fail");

  return(0);
}

//****************************************************************************

