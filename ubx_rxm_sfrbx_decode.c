//****************************************************************************
// From ramblings here
// https://portal.u-blox.com/s/question/0D52p0000DfrTQpCQM/referring-ubxrxmsfrbx-message-to-construct-ubxmgagps-message
//****************************************************************************
//
//  RXM-SFRBX
//  SYS:0 PRN:15 FRQ:-7
//  SUB:2
//   22C34E3C 8B0D38
//   17290A64 5CA429
//   19033893 640CE2
//   0D3944EF 34E513
//   196E9749 65BA5D
//   82DC80D2 0B7203
//   BD63074C F58C1D
//   0541687A 1505A1
//   8383B5A1 0E0ED6
//   91941F94 46507E
//
//  note 0x8B (10001011) leading TLM, and sub-frame# from HOW 0x29 -> 001[010]01
//****************************************************************************

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
//****************************************************************************
 
void DumpUBXRXMSFRBX(BYTE *Buffer)
{
  BYTE *p = Buffer + 6; // Pointer to packet payload
  WORD Payload;
  Payload = *((WORD *)&Buffer[4]); // Size of payload
  if (Payload == 0) // Request Packet
    return;
  printf("RXM-SFRBX\n");
  printf("SYS:%d PRN:%d FRQ:%d\n", p[0], p[1], p[2]-7);
  if ((p[0] == 0x00) && (p[1] >= 1) && (p[1] <= 32)) // GPS, Valid Satellite PRN
  {
    int i;
    DWORD *q = (DWORD *)&p[8]; // subframe words
    DWORD sub = (q[1] >> 8) & 7; // subframe type, from HOW
    printf("SUB:%d\n", sub); // 1,2,3 EPH, 4 ALM1, 5 ALM2
    for(i=0; i<10; i++) // dump subframe words
      printf(" %08X %06X\n", q[i], (q[i] >> 6) & 0xFFFFFF); // raw, parity stripped
  }
} // sourcer32@gmail.com
 
//****************************************************************************
 
int main(int argc, char **argv)
{
  BYTE ubx[] = {
    0xB5,0x62,0x02,0x13,0x30,0x00,
    0x00,0x0F,0x00,0x00,0x0A,0x00,0x01,0x00,
    0x3C,0x4E,0xC3,0x22,0x64,0x0A,0x29,0x17,
    0x93,0x38,0x03,0x19,0xEF,0x44,0x39,0x0D,
    0x49,0x97,0x6E,0x19,0xD2,0x80,0xDC,0x82,
    0x4C,0x07,0x63,0xBD,0x7A,0x68,0x41,0x05,
    0xA1,0xB5,0x83,0x83,0x94,0x1F,0x94,0x91,
    0xC2 }; // missing checksum byte
 
  DumpUBXRXMSFRBX(ubx);
 
  return(1);
}
 
//****************************************************************************
