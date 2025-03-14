// From some conversations, ramblings and postings on the u-Blox forum
//****************************************************************************

// If this saves you several man-hours/days consider https://paypal.me/cliveone
//   sourcer32@gmail.com

// For SendUBLOX code, see
//  https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxChecksum.c

//****************************************************************************

void ConfigRate(void *hSerial, uint32_t RateHz)
{
  uint8_t cfg_rate[] = { // 0x06,0x08 CFG_RATE
    0xB5, 0x62, 0x06, 0x08, 0x06, 0x00,
    0xE8, 0x03, 0x01, 0x00, 0x01, 0x00,
    0xAA, 0xAA }; // compute checksum

  uint32_t Ms = 1000 / RateHz;

  cfg_rate[6 + 0] = (uint8_t)((Ms >>  0) & 0xFF);
  cfg_rate[6 + 1] = (uint8_t)((Ms >>  8) & 0xFF);

  SendUBLOX(hSerial, sizeof(cfg_rate), cfg_rate);
}

//****************************************************************************

void ConfigHNR(void *hSerial, uint32_t RateHz)
{
  uint8_t cfg_hnr[] = { // 0x06,0x5C CFG_HNR
    0xB5, 0x62, 0x06, 0x5C, 0x04, 0x00,
    0x0A, 0x00, 0x00, 0x00,
    0xAA, 0xAA }; // compute checksum

  cfg_hnr[6 + 0] = (uint8_t)RateHz;

  SendUBLOX(hSerial, sizeof(cfg_hnr), cfg_hnr);
}

//****************************************************************************

void ConfigMsgCurrent(void *hSerial, uint16_t Msg, int Enable)
{
  uint8_t ubx_cfg_msg[] = {
    0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, // UBX-CFG-MSG (3-Byte Variant)
    0x00, 0x00, // Message Number
    0x00, // Rate on Current Interface
    0xAA, 0xAA }; // compute checksum

  ubx_cfg_msg[6] = (uint8_t)((Msg >>  8) & 0xFF);
  ubx_cfg_msg[7] = (uint8_t)((Msg >>  0) & 0xFF);

  ubx_cfg_msg[8] = Enable; // Rate

  SendUBLOX(hSerial, sizeof(ubx_cfg_msg), ubx_cfg_msg);
}
//****************************************************************************

#define UBX_PORT_NUMBER_I2C    0
#define UBX_PORT_NUMBER_UART1  1
#define UBX_PORT_NUMBER_UART2  2
#define UBX_PORT_NUMBER_USB    3
#define UBX_PORT_NUMBER_SPI    4

//****************************************************************************

void ConfigBaudEx(void *hSerial, int Port, int Baud, int In, int Out)
{
  uint8_t cfg_prt[] = { // 0x06,0x00 CFG_PRT 8N1
    0xB5,0x62,0x06,0x00,0x14,0x00,
    0x01,0x00,0x00,0x00,0xD0,0x08,0x00,0x00,
    0x00,0x84,0x03,0x00,0x27,0x00,0x23,0x00,
    0x00,0x00,0x00,0x00,
    0xAA,0xAA }; // compute checksum

  cfg_prt[6 +  0] = (uint8_t)Port;

  cfg_prt[6 +  8] = (uint8_t)((Baud >>  0) & 0xFF);
  cfg_prt[6 +  9] = (uint8_t)((Baud >>  8) & 0xFF);
  cfg_prt[6 + 10] = (uint8_t)((Baud >> 16) & 0xFF);
  cfg_prt[6 + 11] = (uint8_t)((Baud >> 24) & 0xFF);

  cfg_prt[6 + 12] = (uint8_t)((In >>  0) & 0xFF);
  cfg_prt[6 + 13] = (uint8_t)((In >>  8) & 0xFF);

  cfg_prt[6 + 14] = (uint8_t)((Out >>  0) & 0xFF);
  cfg_prt[6 + 15] = (uint8_t)((Out >>  8) & 0xFF);

  SendUBLOX(hSerial, sizeof(cfg_prt), cfg_prt);
}

//****************************************************************************

void ConfigMsgPortNumber(void *hSerial, uint16_t Msg, int Enable, int PortNumber)
{
  uint8_t ubx_cfg_msg[] = {
    0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, // UBX-CFG-MSG (8-Byte Variant)
    0x00, 0x00, // Message Number
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // I2C, UART1, UART2, USB, SPI, ??
    0xAA, 0xAA }; // compute checksum

  ubx_cfg_msg[6] = (uint8_t)((Msg >>  8) & 0xFF);
  ubx_cfg_msg[7] = (uint8_t)((Msg >>  0) & 0xFF);

  ubx_cfg_msg[8 + PortNumber] = Enable; // Rate

  SendUBLOX(hSerial, sizeof(ubx_cfg_msg), ubx_cfg_msg);
}

//****************************************************************************

#define UBX_PORT_MASK_I2C    0x01  // 0
#define UBX_PORT_MASK_UART1  0x02  // 1
#define UBX_PORT_MASK_UART2  0x04  // 2
#define UBX_PORT_MASK_USB    0x08  // 3
#define UBX_PORT_MASK_SPI    0x10  // 4

//****************************************************************************

void ConfigMsgPortMask(void *hSerial, uint16_t Msg, int Rate, uint16_t PortMask)
{
  uint8_t ubx_cfg_msg[] = {
    0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, // UBX-CFG-MSG
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // I2C, USART1, USART2, USB, SPI, ??
    0xAA, 0xAA }; // compute checksum
  int i;

  ubx_cfg_msg[6] = (uint8_t)((Msg >>  8) & 0xFF); // Class
  ubx_cfg_msg[7] = (uint8_t)((Msg >>  0) & 0xFF);

  for(i=0; i<6; i++)
    ubx_cfg_msg[8 + i] = (PortMask & (1 << i)) ? Rate : 0;

  SendUBLOX(hSerial, sizeof(ubx_cfg_msg), ubx_cfg_msg);
}

//****************************************************************************

void PollMsg(void *hSerial, uint16_t msg)
{
  uint8_t ubx_poll_msg[] = {
    0xB5, 0x62, 0x00, 0x00, 0x00, 0x00,
    0xAA, 0xAA }; // compute checksum

  ubx_poll_msg[2] = (uint8_t)((msg >>  8) & 0xFF);
  ubx_poll_msg[3] = (uint8_t)((msg >>  0) & 0xFF);

  SendUBLOX(hSerial, sizeof(ubx_poll_msg), ubx_poll_msg);
}

//****************************************************************************

void ConfigDynamic(void *hSerial, int dyn)
{
  uint8_t ubx_cfg_nav5[] = {
    0xB5, 0x62, 0x06, 0x24, 0x24, 0x00,
    0x01, 0x00, // Bit0=Dyn  0xFF, 0xFF, // Flags
    0x03, // Dyn (0=Port,2=Static,3=Ped,4=Car,5=Sea,6=Plane1G,7=Plane2G,8=Plane4G)
    0x03, // 3D
    0x00, 0x00, 0x00, 0x00,
    0x10, 0x27, 0x00, 0x00, //10,000
    0x05, 0x00,  // minElev, drLimit
    0xFA, 0x00, 0xFA, 0x00, // pDop, tDop
    0x64, 0x00, 0x5E, 0x01, // pAcc, tAcc
    0x00, 0x3C, 0x00, 0x00, // staticHoldThresh, dgnssTimeout
    0x10, 0x27, 0x00, 0x00,
    0x00, // utcStandard
    0x00, 0x00, 0x00, 0x00, 0x00, // reserved2[5]
    0xAA, 0xAA }; // compute checksum

  ubx_cfg_nav5[8] = dyn;

  SendUBLOX(hSerial, sizeof(ubx_cfg_nav5), ubx_cfg_nav5);
}

//****************************************************************************

void ConfigElevation(void *hSerial, int elev)
{
  uint8_t ubx_cfg_nav5[] = {
    0xB5, 0x62, 0x06, 0x24, 0x24, 0x00,
    0x02, 0x00, // Bit1=minElv  0xFF, 0xFF, // Flags
    0x03, // Dyn (2=Static,3=Ped,4=Car,5=Sea,6=Plane)
    0x03, // 3D
    0x00, 0x00, 0x00, 0x00,
    0x10, 0x27, 0x00, 0x00, //10,000
    0x05, 0x00,  // minElev, drLimit
    0xFA, 0x00, 0xFA, 0x00, // pDop, tDop
    0x64, 0x00, 0x5E, 0x01, // pAcc, tAcc
    0x00, 0x3C, 0x00, 0x00, // staticHoldThresh, dgnssTimeout
    0x10, 0x27, 0x00, 0x00,
    0x00, // utcStandard
    0x00, 0x00, 0x00, 0x00, 0x00, // reserved2[5]
    0xAA, 0xAA }; // compute checksum

  ubx_cfg_nav5[18] = elev;

  SendUBLOX(hSerial, sizeof(ubx_cfg_nav5), ubx_cfg_nav5);
}

//****************************************************************************

// Simple RAM writes of 1-Bit,1-Byte, 2-Byte and 4-Byte Keys

void ConfigValSet(void *hSerial, uint32_t Key, uint32_t Value)
{
  uint8_t ubx_cfg_valset[] = {
  0xB5,0x62,0x06,0x8A,  // CFG-VALSET
  0xAA,0x00,            // Payload Length
  // Payload
  0x00,0x01,0x00,0x00, // +6 Version 0,RAM=1
  0x00,0x00,0x00,0x00, // +A Key
  0x00,0x00,0x00,0x00, // +E Value [1..4]
  // Checksum
  0xAA,0xAA };
  
  int Size = 8;

  switch(Key >> 28)
  {
    case 1 :
      Value &= 1;
    case 2 :
      Size += 1; break;
    case 3 :
      Size += 2; break;
    case 4 :
      Size += 4; break;
    default : break;
  }

  ubx_cfg_valset[4] = Size;

  ubx_cfg_valset[6 +  4] = (uint8_t)((Key >>  0) & 0xFF);
  ubx_cfg_valset[6 +  5] = (uint8_t)((Key >>  8) & 0xFF);
  ubx_cfg_valset[6 +  6] = (uint8_t)((Key >> 16) & 0xFF);
  ubx_cfg_valset[6 +  7] = (uint8_t)((Key >> 24) & 0xFF);

  ubx_cfg_valset[6 +  8] = (uint8_t)((Value >>  0) & 0xFF);
  ubx_cfg_valset[6 +  9] = (uint8_t)((Value >>  8) & 0xFF);
  ubx_cfg_valset[6 + 10] = (uint8_t)((Value >> 16) & 0xFF);
  ubx_cfg_valset[6 + 11] = (uint8_t)((Value >> 24) & 0xFF);

  SendUBLOX(hSerial, Size+8, ubx_cfg_valset);
}

//****************************************************************************
