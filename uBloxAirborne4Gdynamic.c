// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p00009AaEleCAF/-gps-setting-querying-settings-for-airborne-mode
// https://portal.u-blox.com/s/question/0D52p0000CcNcdYCQS/m8030-setting
//****************************************************************************

// Setting AIRBORNE 4G (DYNAMIC=8), 80 KM VERTICAL, 500 M/S HORIZONTAL DYNAMICS uBlox Series 7 and 8

uint8_t ubx_cfg_nav5[] = { // CFG-NAV5 (06 24)
  0xB5,0x62,0x06,0x24,0x24,0x00, // Header/Command/Size
  0x01,0x00,0x08,0x00,0x00,0x00,0x00,0x00, // Payload data (Dynamics flag, and Dynamic setting)
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,
  0x57,0xF8 }; // Checksum (Fletcher) of preceeding packet

SendUBLOX(hUARTGPS, sizeof(ubx_cfg_nav5), ubx_cfg_nav5);

uint8_t ubx_cfg_rate[] = { // CFG-RATE (06 08) 10 Hz
  0xB5,0x62,0x06,0x08,0x06,0x00, // Header/Command/Size
  0x64,0x00,0x01,0x00,0x01,0x00, 
  0x7A,0x12 }; // Checksum (Fletcher) of preceeding packet

SendUBLOX(hUARTGPS, sizeof(ubx_cfg_rate), ubx_cfg_rate);
