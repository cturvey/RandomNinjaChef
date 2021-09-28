// ISO14443A CRC Example   sourcer32@gmail.com
// Related to conversation on the STM32 Forum, CR95HF CRC
// https://community.st.com/s/question/0D53W000016PRpZSAW/crc-polynomial

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
 
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
 
uint16_t crc_iso14443a(uint16_t crc, uint8_t data)
{
  int i;
  crc = crc ^ data;
  for(i=0; i<8; i++)
    if (crc & 1)
      crc = (crc >> 1) ^ 0x8408;
    else
      crc >>= 1;
  return(crc);
}
 
uint16_t crc_iso14443a_quick(uint16_t crc, uint8_t data)
{
  static const uint16_t crctbl[] = { // 0x8408 Nibble Table, sourcer32@gmail.com
    0x0000,0x1081,0x2102,0x3183,0x4204,0x5285,0x6306,0x7387,
    0x8408,0x9489,0xA50A,0xB58B,0xC60C,0xD68D,0xE70E,0xF78F };
 
  crc = crc ^ data;
 
  crc = (crc >> 4) ^ crctbl[crc & 0xF]; // Two rounds of 4-bits
  crc = (crc >> 4) ^ crctbl[crc & 0xF];
 
  return(crc);
}
 
uint16_t crc_iso14443a_quick_block(uint16_t crc, int size, uint8_t *buffer)
{
  while(size--)
    crc = crc_iso14443a_quick(crc, *buffer++);
 
  return(crc);
}
 
void test_one(void)
{
  // ISO14443-A >>> CR95HFDLL_SENDRECEIVE, 300028
  // << 8015 6A1E4DB17E00004B3592000000000000 9BC3 080000
 
  uint8_t data[] = {
    0x6A,0x1E,0x4D,0xB1,0x7E,0x00,0x00,0x4B,
    0x35,0x92,0x00,0x00,0x00,0x00,0x00,0x00 };
 
  uint16_t crc;
 
  crc = crc_iso14443a_quick_block(0x6363, sizeof(data), data);
 
  printf("test_one=%04X\n", crc );
}
 
void test_two(void)
{
  // ISO14443-A >>> CR95HFDLL_SENDRECEIVE, 300028
  // << 8015 6A1E4DB17E00004B3592000000000000 9BC3 080000
 
  uint8_t data[] = {
    0x6A,0x1E,0x4D,0xB1,0x7E,0x00,0x00,0x4B,
    0x35,0x92,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9B,0xC3 };
 
  uint16_t crc;
 
  crc = crc_iso14443a_quick_block(0x6363, sizeof(data), data);
 
  printf("test_two=%04X\n", crc);
  if (crc == 0) puts("PASS"); else puts("!!FAIL!!");
}
 
int main(int argc, char **argv)
{
  printf("%04X == 0x626D\n", crc_iso14443a(0x6363, 0x12) );
 
  printf("%04X\n", crc_iso14443a_quick(0x6363, 0x12) );
 
  test_one();
  test_two();
 
  return(1);
}
