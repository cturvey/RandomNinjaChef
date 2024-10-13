// Related to musings here
// https://community.st.com/t5/stm32-mcus-products/problem-w-setting-polynomial-for-crc/td-p/730403

// CRC-32K (reversed Koopman polynomial) - 0xEB31D82E
// Input data - 0x31, 0x32, . . ., 0x39
// I expect the resulting CRC to be 0x2D3DD0AE

// Copyright (C) 2024 Clive Turvey (aka Tesla DeLorean, sourcer32@gmail.com)
//  All Rights Reserved

//******************************************************************************

void TestCRC32K(void) // sourcer32@gmail.com PayPal accepted..
{
  uint8_t test1[] = { 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 }; // 0x2D3DD0AE
  uint8_t test2[] = { 0x12,0x34,0x56 };  // 0x85165958

  /* CRC handler declaration */
  CRC_HandleTypeDef CrcHandle = {0};

  /* CRC Peripheral clock enable */
  __HAL_RCC_CRC_CLK_ENABLE();

  /*##-1- Configure the CRC peripheral #######################################*/
  CrcHandle.Instance = CRC;

  /* The default polynomial is not used. It is required to defined it in CrcHandle.Init.GeneratingPolynomial*/
  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE; // ST double-negative, use user supplied

  /* Set the value of the polynomial */
  CrcHandle.Init.GeneratingPolynomial    = 0x741B8CD7; // 32-bit CRC-32-K, bit reversed 0xEB31D82E, hw left shifts, always

  /* The user-defined generating polynomial generates a 32-bit long CRC */
  CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_32B;
	
  /* The default init value is not used */
  CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE; // ST double-negative

  /* The used-defined initialization value */
  CrcHandle.Init.InitValue               = 0xFFFFFFFF; // 32-bit all set

  /* The input data is inverted (bit-reversed) */
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_BYTE; // Bit swap byte end-to-end

  /* The output data is inverted (bit-reversed) */
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;// Bit swap word end-to-end

  /* The input data are 8-bit long */
  CrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;

  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(__FILE__, __LINE__);
  }

  /* Note that I negate the response the hardware spins end-to-end */
  printf("CRC-32-K %08X TEST1 2D3DD0AE ?\n", ~HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test1, sizeof(test1)));
  printf("CRC-32-K %08X TEST1 85165958 ?\n", ~HAL_CRC_Calculate(&CrcHandle, (uint32_t *)test2, sizeof(test2)));
}

//******************************************************************************
// PC side proof-of-concept w/right shift
//******************************************************************************

uint8_t data[] = {
  0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

//******************************************************************************

uint32_t crc32k(uint32_t crc, size_t size, uint8_t *buffer)
{
  int i;
  while(size--)
  {
    crc ^= (uint32_t)*buffer++;
    for(i=0; i<8; i++)
      if (crc & 0x00000001)
        crc = (crc >> 1) ^ 0xEB31D82E;
      else
        crc >>= 1;
  }

  return(crc);
}

//******************************************************************************

int main(int argc, char **argv)
{
  uint32_t crc;

  crc = crc32k(0xFFFFFFFF, sizeof(data), data);
  printf("%08X %08X\n", crc, ~crc);

  return(1);
}

//****************************************************************************
