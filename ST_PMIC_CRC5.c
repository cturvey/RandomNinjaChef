// CRC5 0x05/0x25 Polynomial Example - sourcer32@gmail.com
//  Copyright (C) 2022, C Turvey, All Rights Reserved

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

/*

5-bit CRC for ST Micro PMIC devices protecting 32-bit registers and SPI
 L9369 and L5965
 
Related ramblings
https://community.st.com/s/question/0D53W00001gxfOrSAI/reqeust-for-l9369-crc-calculation-documentation

 If you lift this, remember where you got it, and how much time you wasted previously
  with code and ideas that failed.
 
 If this saves you several man-hours/days consider https://paypal.me/cliveone
 
https://www.st.com/resource/en/datasheet/l5965.pdf

SPI frame CRC generator
The SPI protocol is defined by frames of 32 bits with 5 bits of CRC (Cyclic
Redundancy Check) in both input and output directions. The polynomial
calculation implemented is: g(x)= x^5 + x^2 + 1 the structure of CRC generator
is shown in Figure 25. Structure of CRC generator.

Here are the rules:
1. For DI, CNT=DI[21] is ignored when calculating CRC, it means
only {DI[31:22],DI[20:5]} is used to calculate CRC. For example,
if DI[31:5]=27'b1000_0010_1011_1111_1111_1111_111, the CRC[4:0]= 5'b0_0011

2. For DO, DO[21] is ignored when calculating CRC, it means only
  {DO[31:22],DO[20:5]} is used to calculate CRC.

3. The initial value of CRC generator is 5'b1_1111.

4. MSB (DI[31]) is shift in CRC generator at first.

*/

int main(int argc, char **argv)
{
  int i;
  uint32_t test = 0x82BFFFE0; // 0x03
  uint32_t crc;

  crc = 0x1F << 27; // Initial, aligned high-order to match data

  crc = crc ^ test; // Apply data

  for(i=0; i<27; i++) // Consume data
    if (crc & 0x80000000)
      crc = (crc << 1) ^ (0x05 << 27); // Bitwise application
    else
      crc <<= 1;

  crc = (crc >> 27) & 0x1F; // Recover, align, mask computed value

  printf("CRC:%02X (03?) -> %08X\n", crc, test ^ crc);

  return(1);
}
