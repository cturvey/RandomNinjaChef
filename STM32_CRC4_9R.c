// Related to musings here
// https://community.st.com/t5/stm32-mcus-security/can-the-stm32-crc-peripheral-be-abused-into-doing-crc-24q-or/m-p/65228/highlight/true#M288
// https://community.st.com/t5/stm32-mcus-products/problem-w-setting-polynomial-for-crc/td-p/730403

//****************************************************************************

// This exploits my understanding of the STM32 CRC Hardware
//  Tested on a STM32G474RE on a NUCLEO-G474RE

// 4-bit, Right Shifting, x^4 + x^3 + 1  used in the SPARTN protocol
//  https://www.spartnformat.org/wp-content/uploads/210928_SPARTN_v2.0.1.pdf#page=34
//  Mentioned also at https://users.ece.cmu.edu/~koopman/crc/

// Copyright (C) 2024 Clive Turvey (aka Tesla DeLorean, sourcer32@gmail.com)
//  All Rights Reserved

void TestCRC4_9R(void) // sourcer32@gmail.com PayPal accepted.. gauge by man-hours saved
{
  uint8_t test1[] = { 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 }; // 0x02
  uint8_t test2[] = { 0x12,0x34,0x56 };  // 0x9
	
  /* CRC handler declaration */
  CRC_HandleTypeDef CrcHandle = {0};

  /* CRC Peripheral clock enable */
  __HAL_RCC_CRC_CLK_ENABLE();

  /*##-1- Configure the CRC peripheral #######################################*/
  CrcHandle.Instance = CRC;

  /* The default polynomial is not used. It is required to defined it in CrcHandle.Init.GeneratingPolynomial*/
  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE; // ST double-negative, use user supplied

  /* Set the value of the polynomial */
  CrcHandle.Init.GeneratingPolynomial    = 0x9 << (32 - 4); // 4-bit CRC-4-9R, bit reversed 0x00000009, hw left shifts, always

  /* The user-defined generating polynomial generates a 32-bit long CRC */
  CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_32B;
	
  /* The default init value is not used */
  CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE; // ST double-negative

  /* The used-defined initialization value */
  CrcHandle.Init.InitValue               = 0; // 32-bit all clear

  /* The input data is inverted (bit-reversed) */
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_BYTE; // Bit swap byte end-to-end

  /* The output data is inverted (bit-reversed)*/
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;// Bit swap word end-to-end

  /* The input data are 8-bit long */
  CrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;

  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(__FILE__, __LINE__);
  }

  /* The hardware spins end-to-end, 4-bits will be in least significant bits, shouldn't need to mask
	   as the polynomial adds no bits down there as it consumes the bytes */
  printf("CRC-4-9R %1X TEST1 2 ?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test1, sizeof(test1)) & 0xF);
  printf("CRC-4-9R %1X TEST2 9 ?\n", HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test2, sizeof(test2)) & 0xF);
}

//****************************************************************************//****************************************************************************
