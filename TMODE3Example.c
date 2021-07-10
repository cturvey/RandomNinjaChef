//****************************************************************************
// Quick CFG-TMODE3 Examples for ZED-F9X and NEO-M8P - sourcer32@gmail.com
//****************************************************************************

void ConfigTMODE3Disable(FIFO *Fifo)
{
  BYTE ubx_cfg_tmode3[] = {
    0xB5, 0x62, 0x06, 0x71, 0x28, 0x00,
    0x00, 0x00,             // Version=0, Reserved1
    0x00, 0x00,             // 0=Disable
    0x00, 0x00, 0x00, 0x00, // X
    0x00, 0x00, 0x00, 0x00, // Y
    0x00, 0x00, 0x00, 0x00, // Z
    0x00, 0x00, 0x00, 0x00, // HP X, Y, Z / Reserved2
    0x00, 0x00, 0x00, 0x00, // Acc (cm)
    0xE8, 0x03, 0x00, 0x00, // svinMinDur (seconds/samples) 1000
    0x10, 0x27, 0x00, 0x00, // svinAccLimit (0.1 mm) 10000 = 1m
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Reserved3
    0xAA, 0xAA }; // compute checksum

  SendUBLOX(Fifo, sizeof(ubx_cfg_tmode3), ubx_cfg_tmode3);
}

//****************************************************************************

void ConfigTMODE3SurveyIn(FIFO *Fifo)
{
  BYTE ubx_cfg_tmode3[] = {
    0xB5,0x62,0x06,0x71,0x28,0x00,
    0x00,0x00,           // Version=0, Reserved1
    0x01,0x00,           // 1=Survey In
    0x00,0x00,0x00,0x00, // X
    0x00,0x00,0x00,0x00, // Y
    0x00,0x00,0x00,0x00, // Z
    0x00,0x00,0x00,0x00, // HP X, Y, Z / Reserved2
    0x00,0x00,0x00,0x00, // Acc (cm)
    0x58,0x02,0x00,0x00, // svinMinDur (seconds/samples) 600
    0x88,0x13,0x00,0x00, // svinAccLimit (0.1 mm) 5000 = 0.5m
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // Reserved3
    0xAA,0xAA }; // compute checksum

  SendUBLOX(Fifo, sizeof(ubx_cfg_tmode3), ubx_cfg_tmode3);
}

//****************************************************************************

void ConfigTMODE3LLA(FIFO *Fifo, double lat, double lon, double alt)
{
  BYTE ubx_cfg_tmode3[] = { // sourcer32@gmail.com
    0xB5, 0x62, 0x06, 0x71, 0x28, 0x00,
    0x00, 0x00,             // Version=0, Reserved1
    0x02, 0x01,             // 2=Fixed, 1=Lat/Lot/Alt
    0x00, 0x00, 0x00, 0x00, // X Lat
    0x00, 0x00, 0x00, 0x00, // Y Lon
    0x00, 0x00, 0x00, 0x00, // Z Alt
    0x00, 0x00, 0x00, 0x00, // HP X,Y,Z / Reserved2
    0x0A, 0x00, 0x00, 0x00, // Acc (cm) 10 cm
    0x00, 0x00, 0x00, 0x00, // svinMinDur (seconds/samples)
    0x00, 0x00, 0x00, 0x00, // svinAccLimit (0.1 mm)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Reserved3
    0xAA, 0xAA }; // compute checksum

  int32_t x, y, z;

  x = (int32_t)(lat * 1e7); // Rough Integers
  y = (int32_t)(lon * 1e7);
  z = (int32_t)(alt * 1e2); // m to cm

#if 0
  *((int32_t *)&ubx_cfg_tmode3[6 +  4]) = x; // Course Precision Words
  *((int32_t *)&ubx_cfg_tmode3[6 +  8]) = y;
  *((int32_t *)&ubx_cfg_tmode3[6 + 12]) = z;
#else // Portable on platforms with alignment issues, ie Cortex-M0
  memcpy(&ubx_cfg_tmode3[6 +  4], &x, sizeof(int32_t));
  memcpy(&ubx_cfg_tmode3[6 +  8], &y, sizeof(int32_t));
  memcpy(&ubx_cfg_tmode3[6 + 12], &z, sizeof(int32_t));
#endif

  // Roll off low precision rough integers

  lat -= (double)x * 1e-7;
  lon -= (double)y * 1e-7;
  alt -= (double)z * 1e-2;

  // Compute High-Precision (HP) +/-99 values (might get 100 with rounding)

  x = (int32_t)(lat * 1e9); // Fine Integers
  y = (int32_t)(lon * 1e9);
  z = (int32_t)(alt * 1e4); // m to 0.1 mm

  ubx_cfg_tmode3[6 + 16] = x; // High Precision Bytes
  ubx_cfg_tmode3[6 + 17] = y;
  ubx_cfg_tmode3[6 + 18] = z;

  SendUBLOX(Fifo, sizeof(ubx_cfg_tmode3), ubx_cfg_tmode3);
}

//****************************************************************************

