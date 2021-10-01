// CRC8 0x07 Example   sourcer32@gmail.com
// Related to conversation on the STM32 Forum, CRC-8 / SMBUS
// https://community.st.com/s/question/0D53W000016nFrBSAU/calculated-crc-value-does-not-match-incoming-crc-value

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

uint8_t crc_07(uint8_t crc, uint8_t data)
{
  int i;
  crc = crc ^ data;
  for(i=0; i<8; i++)
    if (crc & 0x80) // Left shifting, MSB
      crc = (crc << 1) ^ 0x07;
    else
      crc <<= 1;
  return(crc);
}

uint8_t crc_07_block(uint8_t crc, int size, uint8_t *buffer)
{
  while(size--)
    crc = crc_07(crc, *buffer++);

  return(crc);
}

uint8_t crc_07_block_quick(uint8_t crc, int size, uint8_t *buffer)
{
  const static uint8_t crctbl[] = {  // 0x07 Nibble Table, sourcer32@gmail.com
    0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,
    0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D };

  while(size--)
  {
    crc ^= *buffer++;

    crc = (crc << 4) ^ crctbl[(crc >> 4) & 0xF]; // Two rounds of 4-bits
    crc = (crc << 4) ^ crctbl[(crc >> 4) & 0xF];
  }

  return(crc);
}

int main(int argc, char **argv)
{
  char test1[] = "123456789";
  uint8_t test2[]= { 0x17, 0x1D, 0x00 };

  printf("%02X\n", crc_07_block(0x00, strlen(test1), (uint8_t *)test1));
  printf("%02X\n", crc_07_block(0x00, sizeof(test2), test2));
  putchar('\n');

  printf("%02X\n", crc_07_block_quick(0x00, strlen(test1), (uint8_t *)test1));
  printf("%02X\n", crc_07_block_quick(0x00, sizeof(test2), test2));
  putchar('\n');

  return(1);
}
