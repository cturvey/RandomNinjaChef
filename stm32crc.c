// STM32 CRC32 Test App - sourcer32@gmail.com
//  Copyright (C) 2014

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

//****************************************************************************

uint32_t Crc32(uint32_t Crc, uint32_t Data)
{
  int i;

  Crc = Crc ^ Data;

  for(i=0; i<32; i++)
    if (Crc & 0x80000000)
      Crc = (Crc << 1) ^ 0x04C11DB7; // Polynomial used in STM32
    else
      Crc = (Crc << 1);

  return(Crc);
}

//****************************************************************************

uint32_t Crc32Block(uint32_t Crc, uint32_t Size, uint32_t *Buffer) // 32-bit units
{
  while(Size--)
    Crc = Crc32(Crc, *Buffer++);

  return(Crc);
}

//****************************************************************************

uint32_t Crc32Fast(uint32_t Crc, uint32_t Data)
{
  static const uint32_t CrcTable[16] = { // Nibble lookup table for 0x04C11DB7 polynomial
    0x00000000,0x04C11DB7,0x09823B6E,0x0D4326D9,0x130476DC,0x17C56B6B,0x1A864DB2,0x1E475005,
    0x2608EDB8,0x22C9F00F,0x2F8AD6D6,0x2B4BCB61,0x350C9B64,0x31CD86D3,0x3C8EA00A,0x384FBDBD };

  Crc = Crc ^ Data; // Apply all 32-bits

  // Process 32-bits, 4 at a time, or 8 rounds
  Crc = (Crc << 4) ^ CrcTable[Crc >> 28]; // Assumes 32-bit reg, masking index to 4-bits
  Crc = (Crc << 4) ^ CrcTable[Crc >> 28]; // 0x04C11DB7 Polynomial used in STM32
  Crc = (Crc << 4) ^ CrcTable[Crc >> 28];
  Crc = (Crc << 4) ^ CrcTable[Crc >> 28];
  Crc = (Crc << 4) ^ CrcTable[Crc >> 28];
  Crc = (Crc << 4) ^ CrcTable[Crc >> 28];
  Crc = (Crc << 4) ^ CrcTable[Crc >> 28];
  Crc = (Crc << 4) ^ CrcTable[Crc >> 28];

  return(Crc);
}

//****************************************************************************

uint32_t Crc32FastBlock(uint32_t Crc, uint32_t Size, uint32_t *Buffer) // 32-bit units
{
  while(Size--)
    Crc = Crc32Fast(Crc, *Buffer++);

  return(Crc);
}

//****************************************************************************

void test(void)
{
  uint8_t vector[12] = { 0x02,0x07,0x02,0x00,0x18,0x8A,0xD0,0x23,0x25,0x2B,0x09,0x00 }; // ACD7E298
  uint32_t Crc;
  int i;

  for(i=0; i<sizeof(vector); i++)
    printf("%02X ",vector[i]);
    putchar('\n');

  Crc = 0xFFFFFFFF; // Initial state

  for(i=0; i<sizeof(vector); i+=4)
  {
    Crc  = Crc32Fast(Crc, *((uint32_t *)&vector[i]) ); // 4-bytes at a time
  }

  printf("%08X %08X test",Crc, Crc32FastBlock(0xFFFFFFFF, sizeof(vector)/4, (void *)vector));
}

//****************************************************************************

void TestFile(char *Filename)
{
  FILE *f;
  uint32_t Size;
  uint8_t *Buffer;

  f = fopen (Filename, "rb");
  if (f)
  {
    fseek(f, 0, SEEK_END);

    Size = ftell (f);

    fseek(f, 0, SEEK_SET);

    if (Size & 3)
      printf("WARNING: File must be multiple of 4 bytes (32-bit) for valid results\n");

    Buffer = malloc(Size);

    fread(Buffer, Size, 1, f);

    fclose(f);

    printf("crc=%08X Slow\n", Crc32Block(0xFFFFFFFF, Size >> 2, (void *)Buffer));
    printf("crc=%08X Fast", Crc32FastBlock(0xFFFFFFFF, Size >> 2, (void *)Buffer));

    free(Buffer);
  }
  else
    printf("ERROR: Unable to open file '%s'\n", Filename);
}

//****************************************************************************

int main(int argc, char **argv)
{
  printf("STM32CRC Test\n  Usage: STM32CRC [<file>]\n");

  if ((Crc32(0xFFFFFFFF, 0x12345678) != 0xDF8A8A2B) ||
      (Crc32Fast(0xFFFFFFFF, 0x12345678) != 0xDF8A8A2B))
  {
    printf("ERROR: Internal Sanity Check Failed\n");
  }

  if (argc > 1)
    TestFile(argv[1]);
  else
    test();

  return(1);
}

//****************************************************************************


