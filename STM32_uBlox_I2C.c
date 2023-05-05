// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p00008Y5pLECAZ/rtcm-to-f9p-via-i2c-bus
//****************************************************************************

uint16_t ublox_ReadLength(void)
{
  uint8_t data[2];
  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&I2CHandle, (0x42 << 1), 0xFD, 1, data, sizeof(data), 100);
  if (status != HAL_OK)
    printf("ublox_ReadLength failed %d %08X\n", status, I2CHandle.ErrorCode);
  return(((uint16_t)data[0] << 8) + (uint16_t)data[1]); // Big Endian
}

//****************************************************************************

void ublox_SendCommand(int size, uint8_t *command)
{
  HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&I2CHandle, (0x42 << 1), 0xFF, 1, command, size, 100);
  if (status != HAL_OK)
    printf("ublox_SendCommand failed %d %08X\n", status, I2CHandle.ErrorCode);
}

//****************************************************************************

void ublox_TestI2C(void)
{
  int i;
  uint8_t ubx_mon_ver[] = { 0xB5,0x62,0x0A,0x04,0x00,0x00,0x0E,0x34 };
  ublox_SendCommand(sizeof(ubx_mon_ver), ubx_mon_ver);
  for(i=0; i<10; i++)
  {
    uint16_t length = ublox_ReadLength();
    printf("uBlox Length %5d %04X\n", length, length);
    if (length)
    {
      static uint8_t buffer[1000];
      uint16_t len = MIN(length, sizeof(buffer));
      HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&I2CHandle, (0x42 << 1), 0xFF, 1, buffer, len, 300);
      if (status == HAL_OK)
        DumpData(len, buffer);
      else
        puts("Read Fail");
    }
    else
      HAL_Delay(1000);
  }
}

//****************************************************************************
