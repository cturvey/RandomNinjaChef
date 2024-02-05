// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p00009AaEleCAF/-gps-setting-querying-settings-for-airborne-mode
//****************************************************************************

// If you happen into the lads from Uputronics or Bodnar Electronics tell them Clive says Hello..

// Setting AIRBOURNE 1G (DYNAMIC=6), 80 KM VERTICAL, 100 M/S HORIZONTAL DYNAMICS uBlox Series 7 and 8

uint8_t ubx_cfg_nav5[] = { // CFG-NAV5 (06 24)
0xB5,0x62,0x06,0x24,0x24,0x00, // Header/Command/Size
0x01,0x00,0x06,0x00,0x00,0x00,0x00,0x00, // Payload data (Dynamics flag, and Dynamic setting)
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x55,0xB4 }; // Checksum (Fletcher) of preceeding packet

SendUBLOX(hUARTGPS, sizeof(ubx_cfg_nav5), ubx_cfg_nav5);

// For Ardunio use the form Serial.write(buffer, sizeof(buffer));
//  to send checksumed packets to the receiver.
