// Related to musings here
// https://community.st.com/t5/stm32-mcus-security/can-the-stm32-crc-peripheral-be-abused-into-doing-crc-24q-or/m-p/65228/highlight/true#M288

//******************************************************************************
 
// Copyright (C) 2022 Clive Turvey (aka Tesla DeLorean, sourcer32@gmail.com)
//  All Rights Reserved
 
void Crc24Test(void) // sourcer32@gmail.com
{
  uint8_t test1[] = { 0x01,0x02,0x03,0x04,0x05,0x06 }; // 0xBC7E06
  uint8_t test2[] = { 0x11,0x22,0x33,0x44,0x55,0x66 }; // 0xD686A9
 
  uint8_t rtcm3testpattern[] = // Packet of known veracity
  { 0xD3,0x00,0x13,0x3E,0xD7,0xD3,0x02,0x02,0x98,0x0E,0xDE,
    0xEF,0x34,0xB4,0xBD,0x62,0xAC,0x09,0x41,0x98,0x6F,0x33,
    0x36,0x0B,0x98 }; // Should be zero when pulled thru polynomial
 
  /* CRC handler declaration */
  CRC_HandleTypeDef CrcHandle = {0};
 
  /* CRC Peripheral clock enable */
  __HAL_RCC_CRC_CLK_ENABLE();
 
  /*##-1- Configure the CRC peripheral #######################################*/
  CrcHandle.Instance = CRC;
 
  /* The default polynomial is not used. It is required to defined it in CrcHandle.Init.GeneratingPolynomial*/
  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
 
  /* Set the value of the polynomial */
  CrcHandle.Init.GeneratingPolynomial    = (0x864CFB << 8); // 24-bit CRC24Q used for SBAS and RTCM3
 
  /* The user-defined generating polynomial generates a 32-bit long CRC */
  CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_32B; // Actually 24-bit we use the high order bits
 
  /* The default init value is not used, want zero */
  CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE;
 
  /* The used-defined initialization value */
  CrcHandle.Init.InitValue               = 0;
 
/* The input data are not inverted */
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;
 
  /* The output data are not inverted */
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
 
  /* The input data are 8-bit long, ie digested byte-wise */
  CrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;
 
  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(__FILE__, __LINE__);
  }
 
  printf("CRC24Q %06X   TEST1 BC7E06?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test1, sizeof(test1)) >> 8); // Get high-order 24-bit computation
 
  printf("CRC24Q %06X   TEST2 D686A9?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test2, sizeof(test2)) >> 8); // Get high-order 24-bit computation
 
  printf("CRC24Q %08X RTCM3 CHECK Zero?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)rtcm3testpattern, sizeof(rtcm3testpattern)));
 
  printf("CRC24Q %06X   RTCM3 COMPUTE 360B98?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)rtcm3testpattern, sizeof(rtcm3testpattern) - 3) >> 8);
}
 
//******************************************************************************
