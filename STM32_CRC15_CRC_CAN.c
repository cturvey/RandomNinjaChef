// Related to musings here
// https://community.st.com/s/question/0D53W00001bIU55SAG/can-the-stm32-crc-peripheral-be-made-to-work-with-the-crc15can-polynomial

//******************************************************************************

// The ADBMS1818 datasheet shows a 15 bit polynomial for the CRC as
//  x^15 + x^14 + x^10 + x^8 + x^7 + x^4 + x^3 + 1
// Other sources call this a CAN-15-CRC polynomial

// As for an example test pattern, we'll be using the ADBMS1818 datasheet
//  example of a two byte {0x00, 0x01} input producing a 0x3D6E output,
//  which uses the polynomical 0x4599 with seed/initial of 0x10, and shifts
//  the 15b result by 1. The high-order 15-bits will already be suitably
//  aligned.

// Copyright (C) 2022 Clive Turvey (aka Tesla DeLorean, sourcer32@gmail.com)
//  All Rights Reserved

void Crc15Test(void) // sourcer32@gmail.com
{
  uint8_t test1[] = {0x00, 0x01 }; // 0x3D6E
  uint8_t test2[] = {0x11, 0x22, 0x33, 0x44, 0x55 }; // 0x7876

  /* CRC handler declaration */
  CRC_HandleTypeDef CrcHandle = {0};

  /* CRC Peripheral clock enable */
  __HAL_RCC_CRC_CLK_ENABLE();

  /*##-1- Configure the CRC peripheral #######################################*/
  CrcHandle.Instance = CRC;

  /* The default polynomial is not used. It is required to defined it in CrcHandle.Init.GeneratingPolynomial*/
  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;

  /* Set the value of the polynomial */
  CrcHandle.Init.GeneratingPolynomial    = (0x4599 << 1); // 15-bit

  /* The user-defined generating polynomial generates a 16-bit long CRC */
  CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_16B; // Actually 15-bit we use the high order bits

  /* The default init value is not used */
  CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE;

  /* The used-defined initialization value */
  CrcHandle.Init.InitValue               = (0x0010 << 1); // 15-bit top aligned

  /* The input data are not inverted */
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;

  /* The output data are not inverted */
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;

  /* The input data are 8-bit long */
  CrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;

  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(__FILE__, __LINE__);
  }

  printf("CRC-15 %04X TEST1 3D6E?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test1, sizeof(test1)));

  printf("CRC-15 %04X TEST2 7876?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test2, sizeof(test2)));
}

//******************************************************************************

// Output test on STM32L4R5ZI

//  CRC-15 3D6E TEST1 3D6E?
//  CRC-15 7876 TEST2 7876?

//******************************************************************************

uint16_t Quick_CRC_Calc15Bits(uint16_t crc, int Size, uint8_t *Buffer)
{
  static const uint16_t CrcTable[] = { // Nibble Table for 0x4599, sourcer32@gmail.com
    0x0000,0xC599,0xCEAB,0x0B32,0xD8CF,0x1D56,0x1664,0xD3FD,
    0xF407,0x319E,0x3AAC,0xFF35,0x2CC8,0xE951,0xE263,0x27FA};

  while(Size--)
  {
    crc = crc ^ (*Buffer++ << (15-8)); // Align upper bits

    crc = (crc << 4) ^ CrcTable[(crc >> (15-4)) & 0xF]; // Process byte 4-bits at a time
    crc = (crc << 4) ^ CrcTable[(crc >> (15-4)) & 0xF];
  }

  return(crc & 0x7FFF); // Table should keep high-order bit clear
}

//******************************************************************************

uint16_t Fast_CRC_Calc15Bits(uint16_t crc, int Size, uint8_t *Buffer)
{
  static const uint16_t CrcTable[] = { // Byte Table for 0x4599, sourcer32@gmail.com
    0x0000,0xC599,0xCEAB,0x0B32,0xD8CF,0x1D56,0x1664,0xD3FD,
    0xF407,0x319E,0x3AAC,0xFF35,0x2CC8,0xE951,0xE263,0x27FA,
    0xAD97,0x680E,0x633C,0xA6A5,0x7558,0xB0C1,0xBBF3,0x7E6A,
    0x5990,0x9C09,0x973B,0x52A2,0x815F,0x44C6,0x4FF4,0x8A6D,
    0x5B2E,0x9EB7,0x9585,0x501C,0x83E1,0x4678,0x4D4A,0x88D3,
    0xAF29,0x6AB0,0x6182,0xA41B,0x77E6,0xB27F,0xB94D,0x7CD4,
    0xF6B9,0x3320,0x3812,0xFD8B,0x2E76,0xEBEF,0xE0DD,0x2544,
    0x02BE,0xC727,0xCC15,0x098C,0xDA71,0x1FE8,0x14DA,0xD143,
    0xF3C5,0x365C,0x3D6E,0xF8F7,0x2B0A,0xEE93,0xE5A1,0x2038,
    0x07C2,0xC25B,0xC969,0x0CF0,0xDF0D,0x1A94,0x11A6,0xD43F,
    0x5E52,0x9BCB,0x90F9,0x5560,0x869D,0x4304,0x4836,0x8DAF,
    0xAA55,0x6FCC,0x64FE,0xA167,0x729A,0xB703,0xBC31,0x79A8,
    0xA8EB,0x6D72,0x6640,0xA3D9,0x7024,0xB5BD,0xBE8F,0x7B16,
    0x5CEC,0x9975,0x9247,0x57DE,0x8423,0x41BA,0x4A88,0x8F11,
    0x057C,0xC0E5,0xCBD7,0x0E4E,0xDDB3,0x182A,0x1318,0xD681,
    0xF17B,0x34E2,0x3FD0,0xFA49,0x29B4,0xEC2D,0xE71F,0x2286,
    0xA213,0x678A,0x6CB8,0xA921,0x7ADC,0xBF45,0xB477,0x71EE,
    0x5614,0x938D,0x98BF,0x5D26,0x8EDB,0x4B42,0x4070,0x85E9,
    0x0F84,0xCA1D,0xC12F,0x04B6,0xD74B,0x12D2,0x19E0,0xDC79,
    0xFB83,0x3E1A,0x3528,0xF0B1,0x234C,0xE6D5,0xEDE7,0x287E,
    0xF93D,0x3CA4,0x3796,0xF20F,0x21F2,0xE46B,0xEF59,0x2AC0,
    0x0D3A,0xC8A3,0xC391,0x0608,0xD5F5,0x106C,0x1B5E,0xDEC7,
    0x54AA,0x9133,0x9A01,0x5F98,0x8C65,0x49FC,0x42CE,0x8757,
    0xA0AD,0x6534,0x6E06,0xAB9F,0x7862,0xBDFB,0xB6C9,0x7350,
    0x51D6,0x944F,0x9F7D,0x5AE4,0x8919,0x4C80,0x47B2,0x822B,
    0xA5D1,0x6048,0x6B7A,0xAEE3,0x7D1E,0xB887,0xB3B5,0x762C,
    0xFC41,0x39D8,0x32EA,0xF773,0x248E,0xE117,0xEA25,0x2FBC,
    0x0846,0xCDDF,0xC6ED,0x0374,0xD089,0x1510,0x1E22,0xDBBB,
    0x0AF8,0xCF61,0xC453,0x01CA,0xD237,0x17AE,0x1C9C,0xD905,
    0xFEFF,0x3B66,0x3054,0xF5CD,0x2630,0xE3A9,0xE89B,0x2D02,
    0xA76F,0x62F6,0x69C4,0xAC5D,0x7FA0,0xBA39,0xB10B,0x7492,
    0x5368,0x96F1,0x9DC3,0x585A,0x8BA7,0x4E3E,0x450C,0x8095 };

  while(Size--)
  {
    crc = crc ^ (*Buffer++ << (15-8)); // Align upper bits

    crc = (crc << 8) ^ CrcTable[(crc >> (15-8)) & 0xFF];
  }

  return(crc & 0x7FFF); // Table should keep high-order bit clear
}

//******************************************************************************

void test1(void)
{
  uint8_t data[] = {0x00, 0x01 }; // 0x3D6E

  printf("crc=%04X Quick\n", Quick_CRC_Calc15Bits(0x0010, sizeof(data), data) << 1); // 0x3D6E

  printf("crc=%04X Fast\n", Fast_CRC_Calc15Bits(0x0010, sizeof(data), data) << 1);
}

//******************************************************************************

void test2(void)
{
  uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55 }; // 0x7876

  printf("crc=%04X Quick\n", Quick_CRC_Calc15Bits(0x0010, sizeof(data), data) << 1); // 0x7876

  printf("crc=%04X Fast\n", Fast_CRC_Calc15Bits(0x0010, sizeof(data), data) << 1);
}

//******************************************************************************
