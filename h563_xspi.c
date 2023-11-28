/*

 STM32H563 / H573 XSPI Command Initialization Example for W25Qxxx QuadSPI devices
 
 If you lift this, remember where you got it, and how much time you wasted previously
  with code and ideas that failed.
 
 If this saves you several man-hours/days consider https://paypal.me/cliveone

 27-Nov-2023
 -Clive

*/

#define READ_STATUS_REG1_CMD		0x05
#define READ_STATUS_REG2_CMD 		0x35
#define READ_STATUS_REG3_CMD 		0x15

//****************************************************************************

static uint8_t XSPI_ReadStatus(XSPI_HandleTypeDef *hospi, uint32_t Reg, uint32_t *pStatusReg)
{
  XSPI_RegularCmdTypeDef sCommand = {0};
  uint32_t data = 0;

  sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  sCommand.IOSelect           = HAL_XSPI_SELECT_IO_3_0;
  sCommand.DataLength         = 1;

  switch(Reg)
  {
    case 0 : sCommand.Instruction = READ_STATUS_REG1_CMD; break; // REG1
    case 1 : sCommand.Instruction = READ_STATUS_REG2_CMD; break; // REG2
    case 2 : sCommand.Instruction = READ_STATUS_REG3_CMD; break; // REG3
    case 0x9F : // READID
      sCommand.Instruction   = 0x9F;
      sCommand.DataLength    = 3;
      break;
  }

  sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionWidth   = HAL_XSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressMode        = HAL_XSPI_ADDRESS_NONE;
  sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  sCommand.DataMode           = HAL_XSPI_DATA_1_LINE;
  sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles        = 0;
  sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode           = HAL_XSPI_SIOO_INST_EVERY_CMD;

  if (HAL_XSPI_Command(hospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return(XSPI_ERROR);
  }

  if (HAL_XSPI_Receive(hospi, (void *)&data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return(XSPI_ERROR);
  }

  if (pStatusReg)
    *pStatusReg = data;

  return(XSPI_OK);
}

//****************************************************************************
