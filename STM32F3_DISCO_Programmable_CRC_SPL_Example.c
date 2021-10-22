//****************************************************************************
// STM32F3-DISCO Programmable CRC test/validation suite - sourcer32@gmail.com
//
// Demonstrates common modes of utilization, please email if you'd like to
// discuss specific use cases, optimizations or different CRC polynomials.
//
// Programmable CRC also available on the STM32F072B-DISCO
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>

#include "stm32f3_discovery.h"

//****************************************************************************
// STM32F3-Discovery SWO - Make SB10 for SWV (Serial Wire Viewer)
//****************************************************************************

static int Debug_ITMDebug = 0;

//****************************************************************************

void Debug_ITMDebugEnable(void)
{
  volatile unsigned int *ITM_TER      = (volatile unsigned int *)0xE0000E00;
  volatile unsigned int *SCB_DHCSR    = (volatile unsigned int *)0xE000EDF0;
  volatile unsigned int *DBGMCU_CR    = (volatile unsigned int *)0xE0042004;

  *DBGMCU_CR |= 0x27; // DBGMCU_CR

  if ((*SCB_DHCSR & 1) && (*ITM_TER & 1)) // Enabled?
    Debug_ITMDebug = 1;
}

//****************************************************************************

void Debug_ITMDebugOutputChar(char ch)
{
  static volatile unsigned int *ITM_STIM0 = (volatile unsigned int *)0xE0000000; // ITM Port 0
  static volatile unsigned int *SCB_DEMCR = (volatile unsigned int *)0xE000EDFC;

  if (Debug_ITMDebug && (*SCB_DEMCR & 0x01000000))
  {
    while(*ITM_STIM0 == 0);
    *((volatile char *)ITM_STIM0) = ch;
  }
}

//****************************************************************************

void Debug_ITMDebugOutputString(char *Buffer)
{
  if (Debug_ITMDebug)
    while(*Buffer)
      Debug_ITMDebugOutputChar(*Buffer++);
}

//******************************************************************************

static void CRC_PK32Test(void);
static void CRC_MODBUS16Test(void);
static void CRC_CRC16Test(void);

//******************************************************************************

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f30x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f30x.c file
     */

  Debug_ITMDebugEnable();

  printf("STM32 Programmable CRC Demo\n");


  CRC_PK32Test();
  CRC_MODBUS16Test();
  CRC_CRC16Test();


  while(1); /* Forever loop */
}

//******************************************************************************

// Transfers blocks, optimizes for 32-bit, 16-bit and 8-bit transfers

uint32_t CRC_HardwareBlock(int Size, const uint8_t *Buffer)
{
  while(Size >= 4) // Do as much as we can at 32-bits
  {
    CRC->DR = *((uint32_t *)Buffer);

    Buffer += 4;
    Size -= 4;
  }

  while(Size >= 2) // Perhaps one of these
  {
    *((uint16_t *)&CRC->DR) = *((uint16_t *)Buffer);

    Buffer += 2;
    Size -= 2;
  }

  while(Size--) // Perhaps one of these
    *((uint8_t *)&CRC->DR) = *Buffer++;

  return(CRC->DR); // Return final CRC computation
}

//******************************************************************************

// Transfers that must be done a byte at a time to manage the endian issues
//  32-bit words could be handled using REV instruction on architectures that
//  support it.

uint32_t CRC_HardwareBlockBytewise(int Size, const uint8_t *Buffer)
{
  while(Size--)
    *((uint8_t *)&CRC->DR) = *Buffer++;

  return(CRC->DR); // Return final CRC computation
}

//******************************************************************************

// PK ZIP compatible 32-bit CRC computation
//  Correct initial conditions, shift direction, polynomial, and final treatment

uint32_t CRC_PK32Slow(uint32_t Crc, uint8_t Data)
{
  int i;

  Crc = Crc ^ (uint32_t)Data;

  for(i=0; i<8; i++)
    if (Crc & 0x00000001)
      Crc = (Crc >> 1) ^ 0xEDB88320;
    else
      Crc >>= 1;

  return(Crc);
}

//******************************************************************************

uint32_t CRC_PK32SlowBlock(uint32_t Crc, int Size, const uint8_t *Buffer)
{
  while(Size--)
    Crc = CRC_PK32Slow(Crc, *Buffer++);

  return(Crc);
}

//****************************************************************************

uint32_t CRC_PK32FastBlock(uint32_t Crc, int Size, const uint8_t *Buffer)
{
  static const uint32_t CrcTable[] = { // For 0xEDB88320 Polynomial
    0x00000000,0x1DB71064,0x3B6E20C8,0x26D930AC,0x76DC4190,0x6B6B51F4,0x4DB26158,0x5005713C,
    0xEDB88320,0xF00F9344,0xD6D6A3E8,0xCB61B38C,0x9B64C2B0,0x86D3D2D4,0xA00AE278,0xBDBDF21C };

  while(Size--)
  {
    Crc = Crc ^ (uint32_t)*Buffer++;

    Crc = (Crc >> 4) ^ CrcTable[Crc & 0x0F]; // Process 8-bits, Two rounds of 4-bits
    Crc = (Crc >> 4) ^ CrcTable[Crc & 0x0F];
  }

  return(Crc);
}

//******************************************************************************

// Test fixture, demo software computation, and hardware configuration

static void CRC_PK32Test(void)
{
  static const uint8_t test[] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B };
  static const uint32_t ValidCrc = 0x8222BEE6;
  uint32_t Crc;

  Crc = ~CRC_PK32SlowBlock(0xFFFFFFFF, sizeof(test), test);

  printf("%08X PK32 Slow\n", Crc);

  if (Crc != ValidCrc)
    puts("Slow software solution invalid");

  Crc = ~CRC_PK32FastBlock(0xFFFFFFFF, sizeof(test), test);

  printf("%08X PK32 Fast\n", Crc);

  if (Crc != ValidCrc)
    puts("Fast software solution invalid");

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE); // Enable CRC AHB clock interface

  CRC_DeInit(); // DeInit CRC peripheral

  CRC_SetInitRegister(0xFFFFFFFF); // Set reset value to all ones

  CRC_PolynomialSizeSelect(CRC_PolSize_32); // Select 32-bit polynomial size

  CRC_ReverseInputDataSelect(CRC_ReverseInputData_32bits); // Reverse bits end-to-end

  CRC_ReverseOutputDataCmd(ENABLE); // Reverse bits end-to-end

  CRC_SetPolynomial(0x04C11DB7); // Reverse of right shifting 0xEDB88320, STM32 always left shifts

  CRC_ResetDR(); // Reset the CRC register

  Crc = ~CRC_HardwareBlock(sizeof(test), test); // Invert CRC

  printf("%08X PK32 Hardware\n", Crc);

  if (Crc != ValidCrc)
    puts("Hardware solution invalid");
}

//******************************************************************************

// MODBUS compatible 16-bit CRC computation - 16-bit right shifting demo

// Byte level processing

uint16_t CRC_MODBUS16Slow(uint16_t Crc, uint8_t Data)
{
  int i;

  Crc = Crc ^ Data;

  for(i=0; i<8; i++)
    if (Crc & 0x0001)
      Crc = (Crc >> 1) ^ 0xA001; // MODBUS CRC16
    else
      Crc = (Crc >> 1);

  return(Crc);
}

//******************************************************************************

// Block level processing

uint16_t CRC_MODBUS16SlowBlock(uint16_t Crc, int Size, const uint8_t *Buffer)
{
  while(Size--)
    Crc = CRC_MODBUS16Slow(Crc, *Buffer++);

  return(Crc);
}

//******************************************************************************

uint16_t CRC_MODBUS16Quick(uint16_t Crc, uint8_t Data)
{
  static const uint16_t CrcTable[] = { // Nibble Table, Right Shifting, 0xA001 Polynomial - sourcer32@gmail.com
    0x0000,0xCC01,0xD801,0x1400,0xF001,0x3C00,0x2800,0xE401,
    0xA001,0x6C00,0x7800,0xB401,0x5000,0x9C01,0x8801,0x4400 };

  Crc = Crc ^ Data;

  Crc = (Crc >> 4) ^ CrcTable[Crc & 0xF]; // Process 8-bits, Two rounds of 4-bits
  Crc = (Crc >> 4) ^ CrcTable[Crc & 0xF];

  return(Crc);
}

//******************************************************************************

uint16_t CRC_MODBUS16QuickBlock(uint16_t Crc, int Size, const uint8_t *Buffer)
{
  while(Size--)
    Crc = CRC_MODBUS16Quick(Crc, *Buffer++);

  return(Crc);
}

//******************************************************************************

uint16_t CRC_MODBUS16Fast(uint16_t Crc, uint8_t Data)
{
  static const uint16_t CrcTable[] = { // Byte Table, Right Shifting, 0xA001 Polynomial - sourcer32@gmail.com
    0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,
    0xC601,0x06C0,0x0780,0xC741,0x0500,0xC5C1,0xC481,0x0440,
    0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,0xCE81,0x0E40,
    0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,
    0xD801,0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,
    0x1E00,0xDEC1,0xDF81,0x1F40,0xDD01,0x1DC0,0x1C80,0xDC41,
    0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,0xD641,
    0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,
    0xF001,0x30C0,0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,
    0x3600,0xF6C1,0xF781,0x3740,0xF501,0x35C0,0x3480,0xF441,
    0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,
    0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,
    0x2800,0xE8C1,0xE981,0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,
    0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,0xEC81,0x2C40,
    0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,
    0x2200,0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,
    0xA001,0x60C0,0x6180,0xA141,0x6300,0xA3C1,0xA281,0x6240,
    0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,0xA441,
    0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,
    0xAA01,0x6AC0,0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,
    0x7800,0xB8C1,0xB981,0x7940,0xBB01,0x7BC0,0x7A80,0xBA41,
    0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,
    0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,
    0x7200,0xB2C1,0xB381,0x7340,0xB101,0x71C0,0x7080,0xB041,
    0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,0x5280,0x9241,
    0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,
    0x9C01,0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,
    0x5A00,0x9AC1,0x9B81,0x5B40,0x9901,0x59C0,0x5880,0x9841,
    0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,0x4A40,
    0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,
    0x4400,0x84C1,0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,
    0x8201,0x42C0,0x4380,0x8341,0x4100,0x81C1,0x8081,0x4040 };

  Crc = Crc ^ Data;

  Crc = (Crc >> 8) ^ CrcTable[Crc & 0xFF]; // Process 8-bits at a time

  return(Crc);
}

//******************************************************************************

uint16_t CRC_MODBUS16FastBlock(uint16_t Crc, int Size, const uint8_t *Buffer)
{
  while(Size--)
    Crc = CRC_MODBUS16Fast(Crc, *Buffer++);

  return(Crc);
}

//******************************************************************************

// Test fixture, demo software computation, and hardware configuration

static void CRC_MODBUS16Test(void)
{
  static const uint8_t test[] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B };
  static const uint16_t ValidCrc = 0x72C3;
  uint16_t Crc;

  Crc = CRC_MODBUS16SlowBlock(0xFFFF, sizeof(test), test);

  printf("%04X MODBUS16 Slow\n", Crc);

  if (Crc != ValidCrc)
    puts("Slow software solution invalid");

  Crc = CRC_MODBUS16QuickBlock(0xFFFF, sizeof(test), test);

  printf("%04X MODBUS16 Quick\n", Crc);

  if (Crc != ValidCrc)
    puts("Quick software solution invalid");

  Crc = CRC_MODBUS16FastBlock(0xFFFF, sizeof(test), test);

  printf("%04X MODBUS16 Fast\n", Crc);

  if (Crc != ValidCrc)
    puts("Fast software solution invalid");

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE); // Enable CRC AHB clock interface

  CRC_DeInit(); // DeInit CRC peripheral

  CRC_SetInitRegister(0xFFFF); // Set reset value to all ones

  CRC_PolynomialSizeSelect(CRC_PolSize_16); // Select 32-bit polynomial Size

  CRC_ReverseInputDataSelect(CRC_ReverseInputData_32bits); // Reverse bits end-to-end

  CRC_ReverseOutputDataCmd(ENABLE); // Reverse bits end-to-end

  CRC_SetPolynomial(0x8005); // Reverse of right shifting 0xA001, STM32 always left shifts

  CRC_ResetDR(); // Reset the CRC register

  Crc = (uint16_t)CRC_HardwareBlock(sizeof(test), test);

  printf("%04X MODBUS16 Hardware\n", Crc);

  if (Crc != ValidCrc)
    puts("Hardware solution invalid");
}

//******************************************************************************

// XMODEM compatible 16-bit CRC computation - 16-bit left shifting demo

// Block level processing

uint16_t CRC_CRC16SlowBlock(uint16_t Crc, int Size, const uint8_t *Buffer)
{
  int i;

  while(Size--)
  {
    Crc = Crc ^ ((uint16_t)*Buffer++ << 8);

    for(i=0; i<8; i++)
      if (Crc & 0x8000)
        Crc = (Crc << 1) ^ 0x1021;
      else
        Crc = (Crc << 1);
  }

  return(Crc);
}

//******************************************************************************

uint16_t CRC_CRC16QuickBlock(uint16_t Crc, int Size, const uint8_t *Buffer)
{
  static const uint16_t CrcTable[] = { // Nibble Table, Left Shifting, 0x1021 Polynomial - sourcer32@gmail.com
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,
    0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF };

  while(Size--)
  {
    Crc = Crc ^ ((uint16_t)*Buffer++ << 8);

    Crc = (Crc << 4) ^ CrcTable[Crc >> 12]; // Process 8-bits, Two rounds of 4-bits
    Crc = (Crc << 4) ^ CrcTable[Crc >> 12];
  }

  return(Crc);
}

//******************************************************************************

uint16_t CRC_CRC16FastBlock(uint16_t Crc, int Size, const uint8_t *Buffer)
{
  static const uint16_t CrcTable[] = { // Byte Table, 0x1021 Left Shifting Polynomial - sourcer32@gmail.com
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,
    0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,
    0x1231,0x0210,0x3273,0x2252,0x52B5,0x4294,0x72F7,0x62D6,
    0x9339,0x8318,0xB37B,0xA35A,0xD3BD,0xC39C,0xF3FF,0xE3DE,
    0x2462,0x3443,0x0420,0x1401,0x64E6,0x74C7,0x44A4,0x5485,
    0xA56A,0xB54B,0x8528,0x9509,0xE5EE,0xF5CF,0xC5AC,0xD58D,
    0x3653,0x2672,0x1611,0x0630,0x76D7,0x66F6,0x5695,0x46B4,
    0xB75B,0xA77A,0x9719,0x8738,0xF7DF,0xE7FE,0xD79D,0xC7BC,
    0x48C4,0x58E5,0x6886,0x78A7,0x0840,0x1861,0x2802,0x3823,
    0xC9CC,0xD9ED,0xE98E,0xF9AF,0x8948,0x9969,0xA90A,0xB92B,
    0x5AF5,0x4AD4,0x7AB7,0x6A96,0x1A71,0x0A50,0x3A33,0x2A12,
    0xDBFD,0xCBDC,0xFBBF,0xEB9E,0x9B79,0x8B58,0xBB3B,0xAB1A,
    0x6CA6,0x7C87,0x4CE4,0x5CC5,0x2C22,0x3C03,0x0C60,0x1C41,
    0xEDAE,0xFD8F,0xCDEC,0xDDCD,0xAD2A,0xBD0B,0x8D68,0x9D49,
    0x7E97,0x6EB6,0x5ED5,0x4EF4,0x3E13,0x2E32,0x1E51,0x0E70,
    0xFF9F,0xEFBE,0xDFDD,0xCFFC,0xBF1B,0xAF3A,0x9F59,0x8F78,
    0x9188,0x81A9,0xB1CA,0xA1EB,0xD10C,0xC12D,0xF14E,0xE16F,
    0x1080,0x00A1,0x30C2,0x20E3,0x5004,0x4025,0x7046,0x6067,
    0x83B9,0x9398,0xA3FB,0xB3DA,0xC33D,0xD31C,0xE37F,0xF35E,
    0x02B1,0x1290,0x22F3,0x32D2,0x4235,0x5214,0x6277,0x7256,
    0xB5EA,0xA5CB,0x95A8,0x8589,0xF56E,0xE54F,0xD52C,0xC50D,
    0x34E2,0x24C3,0x14A0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xA7DB,0xB7FA,0x8799,0x97B8,0xE75F,0xF77E,0xC71D,0xD73C,
    0x26D3,0x36F2,0x0691,0x16B0,0x6657,0x7676,0x4615,0x5634,
    0xD94C,0xC96D,0xF90E,0xE92F,0x99C8,0x89E9,0xB98A,0xA9AB,
    0x5844,0x4865,0x7806,0x6827,0x18C0,0x08E1,0x3882,0x28A3,
    0xCB7D,0xDB5C,0xEB3F,0xFB1E,0x8BF9,0x9BD8,0xABBB,0xBB9A,
    0x4A75,0x5A54,0x6A37,0x7A16,0x0AF1,0x1AD0,0x2AB3,0x3A92,
    0xFD2E,0xED0F,0xDD6C,0xCD4D,0xBDAA,0xAD8B,0x9DE8,0x8DC9,
    0x7C26,0x6C07,0x5C64,0x4C45,0x3CA2,0x2C83,0x1CE0,0x0CC1,
    0xEF1F,0xFF3E,0xCF5D,0xDF7C,0xAF9B,0xBFBA,0x8FD9,0x9FF8,
    0x6E17,0x7E36,0x4E55,0x5E74,0x2E93,0x3EB2,0x0ED1,0x1EF0 };

  while(Size--)
  {
    Crc = Crc ^ ((uint16_t)*Buffer++ << 8);

    Crc = (Crc << 8) ^ CrcTable[Crc >> 8]; // Process 8-bits at a time
  }

  return(Crc);
}

//******************************************************************************

// Test fixture, demo software computation, and hardware configuration

static void CRC_CRC16Test(void)
{
  static const uint8_t test[] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B };
  static const uint16_t ValidCrc = 0x2685;
  uint16_t Crc;

  Crc = CRC_CRC16SlowBlock(0xFFFF, sizeof(test), test);

  printf("%04X CRC16 Slow\n", Crc);

  if (Crc != ValidCrc)
    puts("Slow software solution invalid");

  Crc = CRC_CRC16QuickBlock(0xFFFF, sizeof(test), test);

  printf("%04X CRC16 Quick\n", Crc);

  if (Crc != ValidCrc)
    puts("Quick software solution invalid");

  Crc = CRC_CRC16FastBlock(0xFFFF, sizeof(test), test);

  printf("%04X CRC16 Fast\n", Crc);

  if (Crc != ValidCrc)
    puts("Fast software solution invalid");

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE); // Enable CRC AHB clock interface

  CRC_DeInit(); // DeInit CRC peripheral

  CRC_SetInitRegister(0xFFFF); // Set reset value to all ones

  CRC_PolynomialSizeSelect(CRC_PolSize_16); // Select 32-bit polynomial Size

  CRC_SetPolynomial(0x1021); // Left shifting 0x1021, STM32 always left shifts

  CRC_ResetDR(); // Reset the CRC register

  Crc = (uint16_t)CRC_HardwareBlockBytewise(sizeof(test), test); // STM32 endian backward for left shifting

  printf("%04X CRC16 Hardware\n", Crc);

  if (Crc != ValidCrc)
    puts("Hardware solution invalid");
}

//****************************************************************************
// Hosting of stdio functionality through SWV on Keil - STM32F3-DISCO make SB10
//****************************************************************************

#include <rt_misc.h>

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)
{
  Debug_ITMDebugOutputChar(ch);

  return(ch);
}

int fgetc(FILE *f)
{
  char ch;

ch = 1;

  return((int)ch);
}

int ferror(FILE *f)
{
  /* Your implementation of ferror */
  return EOF;
}

void _ttywrch(int ch)
{
  Debug_ITMDebugOutputChar(ch);
}

void _sys_exit(int return_code)
{
label:  goto label;  /* endless loop */
}

//****************************************************************************

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

//****************************************************************************
