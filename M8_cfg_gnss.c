// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p00008HKEEYCA5/ublox-gps-galileo-enabling-for-ubx-m8
// https://portal.u-blox.com/s/question/0D52p0000BLc5JKCQZ/m8n-write-config-cfg-edited
//****************************************************************************

uint8_t ubx_cfg_gnss[] = { // Full spectrum enabling GPS, GLONASS, SBAS, QZSS and GALILEO, disable BEIDOU/IMES
  0xB5,0x62,0x06,0x3E,0x3C,0x00, // Header/Command/Size [UBX-CFG-GNSS (06 3E)]
  0x00,0x00,0x20,0x07, // Version, HW Channels (Read-Only), Useable (Read-Only), Blocks following..
  0x00,0x08,0x10,0x00,0x01,0x00,0x01,0x01, // GPS, Min/Max Channel Resources, ENABLED, L1, BIT24 (per uCenter/Query, cut-n-paste of hex frame)
  0x01,0x01,0x03,0x00,0x01,0x00,0x01,0x01, // SBAS
  0x02,0x04,0x08,0x00,0x01,0x00,0x01,0x01, // GALILEO
  0x03,0x08,0x10,0x00,0x00,0x00,0x01,0x01, // BEIDOU (DISABLED)
  0x04,0x00,0x08,0x00,0x00,0x00,0x01,0x01, // IMES (DISABLED)
  0x05,0x00,0x03,0x00,0x01,0x00,0x01,0x01, // QZSS
  0x06,0x08,0x0E,0x00,0x01,0x00,0x01,0x01, // GLONASS
  0x30,0xAD }; // Fletcher checksum, correct for preceeding frame

SendUBLOX(hUARTGPS, sizeof(ubx_cfg_gnss), ubx_cfg_gnss); // Use sizeof() for array, strlen() will catch the NULs

// As escaped characters
// "\xB5\x62\x06\x3E\x3C\x00\x00\x00\x20\x07\x00\x08\x10\x00\x01\x00\x01\x01\x01\x01\x03\x00\x01\x00\x01\x01\x02\x04\x08\x00\x01\x00\x01\x01\x03\x08\x10\x00\x00\x00\x01\x01\x04\x00\x08\x00\x00\x00\x01\x01\x05\x00\x03\x00\x01\x00\x01\x01\x06\x08\x0E\x00\x01\x00\x01\x01\x30\xAD"

// The M8 supports Two RF down-converters, and this limits the constellations it will support concurrently
//   1575.42 MHz  GPS+SBAS+GALILEO+QZSS
//   1602 MHz  GLONASS
//   1561.098 MHz  BEIDOU
  
// Related resources
// https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
// https://content.u-blox.com/sites/default/files/u-blox-M9-SPG-4.04_InterfaceDescription_UBX-21022436.pdf?hash=undefined
