// Related to musings here
// https://community.st.com/t5/stm32-mcus-security/can-the-stm32-crc-peripheral-be-abused-into-doing-crc-24q-or/m-p/65228/highlight/true#M288

// This is a quick proof-of-concept, not sure of CANFD's initialization,and final treatment
//  17-bit, left-shifting, x^17 + x^16 + x^14 + x^13 + x^11 + x^6 + x^4 + x^3 + x^1 + 1

// Copyright (C) 2024 Clive Turvey (aka Tesla DeLorean, sourcer32@gmail.com)
//  All Rights Reserved

//****************************************************************************

void TestCrc17CAN(void) // sourcer32@gmail.com PayPal accepted..
{
  uint8_t test1[] = { 0x01,0x02,0x03,0x04,0x05,0x06 }; // 0x176AB
  uint8_t test2[] = { 0x11,0x22,0x33,0x44,0x55,0x66 }; // 0x0FFAF
  uint8_t test3[] = { 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 }; // 0x04F03
 
  /* CRC handler declaration */
  CRC_HandleTypeDef CrcHandle = {0};
 
  /* CRC Peripheral clock enable */
  __HAL_RCC_CRC_CLK_ENABLE();
 
  /*##-1- Configure the CRC peripheral #######################################*/
  CrcHandle.Instance = CRC;
 
  /* The default polynomial is not used. It is required to defined it in CrcHandle.Init.GeneratingPolynomial*/
  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
 
  /* Set the value of the polynomial */
  CrcHandle.Init.GeneratingPolynomial    = (0x1685B << (32-17)); // 17-bit CRC17CAN used for CANFD
 
  /* The user-defined generating polynomial generates a 32-bit long CRC */
  CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_32B; // Actually 17-bit we use the high order bits
 
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
 
  printf("CRC17CAN %05X   TEST1 176AB ?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test1, sizeof(test1)) >> (32-17)); // Get high-order 17-bit computation
 
  printf("CRC17CAN %05X   TEST2 0FFAF ?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test2, sizeof(test2)) >> (32-17)); // Get high-order 17-bit computation
 
  printf("CRC17CAN %05X   TEST3 04F03 ?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test3, sizeof(test3)) >> (32-17)); // Get high-order 17-bit computation
}

//****************************************************************************//****************************************************************************
