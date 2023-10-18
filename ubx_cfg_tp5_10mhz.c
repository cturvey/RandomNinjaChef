// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p0000Dvnnd6CQA/time-pulse-output-specs-at-high-frequency
// https://portal.u-blox.com/s/question/0D52p0000E4fdGuCQI/asking-help-to-generate-the-10mhz-time-pulse
//****************************************************************************

uint8_t ubx-cfg-tp5[] = { // 10MHz 50/50 TIMEPULSE1
0xB5,0x62,0x06,0x31,0x20,0x00,  // Header/Command/Size [UBX-CFG-TP5 (06 31)]
0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x96,0x98,0x00,0x80,0x96,0x98,0x00,
0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x6F,0x00,0x00,0x00,
0x23,0x02 }; // Fletcher checksum, correct for preceeding frame

//****************************************************************************

SendUBLOX(hUARTGPS, sizeof(ubx_cfg_tp5), ubx_cfg_tp5); // Use sizeof() for array, strlen() will catch the NULs
