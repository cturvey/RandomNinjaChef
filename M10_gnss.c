// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D5Oj00000LYRCcKAP/how-to-enable-constellations-on-m10q
//****************************************************************************

/*
 Full Spectrum GNSS on u-Blox M10 parts

 If this saves you several man-hours/days consider https://paypal.me/cliveone
  20-Sep-2024  sourcer32@gmail.com
*/

uint8_t M10GNSS[] = { // GPS:L1C/A,SBAS:L1C/A,GAL:E1,BDS:B1C,GLO:L1,QZSS:L1C/A,L1S
  0xB5,0x62,0x06,0x8A,0x4A,0x00, // UBX-CFG-VALSET
  0x01,0x01,0x00,0x00, // RAM
  0x01,0x00,0x31,0x10,0x01, // CFG-SIGNAL-GPS_L1CA_ENA  = 1
  0x05,0x00,0x31,0x10,0x01, // CFG-SIGNAL-SBAS_L1CA_ENA  = 1
  0x07,0x00,0x31,0x10,0x01, // CFG-SIGNAL-GAL_E1_ENA  = 1
  0x0D,0x00,0x31,0x10,0x00, // CFG-SIGNAL-BDS_B1_ENA  = 0
  0x12,0x00,0x31,0x10,0x01, // CFG-SIGNAL-QZSS_L1CA_ENA  = 1
  0x14,0x00,0x31,0x10,0x01, // CFG-SIGNAL-QZSS_L1S_ENA  = 1
  0x18,0x00,0x31,0x10,0x01, // CFG-SIGNAL-GLO_L1_ENA  = 1
  0x1F,0x00,0x31,0x10,0x01, // CFG-SIGNAL-GPS_ENA  = 1
  0x20,0x00,0x31,0x10,0x01, // CFG-SIGNAL-SBAS_ENA  = 1
  0x21,0x00,0x31,0x10,0x01, // CFG-SIGNAL-GAL_ENA  = 1
  0x22,0x00,0x31,0x10,0x01, // CFG-SIGNAL-BDS_ENA  = 1
  0x24,0x00,0x31,0x10,0x01, // CFG-SIGNAL-QZSS_ENA  = 1
  0x25,0x00,0x31,0x10,0x01, // CFG-SIGNAL-GLO_ENA  = 1
  0x0F,0x00,0x31,0x10,0x01, // CFG-SIGNAL-BDS_B1C_ENA  = 1
  0xA9,0x4D }; // Fletcher checksum, correct for preceeding frame

SendUBLOX(hUARTGPS, sizeof(M10GNSS), M10GNSS); // Use sizeof() for array, strlen() will catch the NULs

// For Checksum / Send mechanics see
//   https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxChecksum.c
