//****************************************************************************
// Quick CFG-TMODE2 Examples for LEA-M8F and LEA/NEO-M8T - sourcer32@gmail.com
//****************************************************************************
// Reporting via UBX-TIM-SVIN, Enable current interface
//   B5 62 06 01 03 00 0D 04 01 1C 6F
//****************************************************************************

void ConfigTMODE2Disable(FIFO *Fifo) // Tentitive, based on TMODE3 example
{
  BYTE ubx_cfg_tmode2[] = { // CFG-TMODE2 (06 3D)
    0xB5,0x62,0x06,0x3D,0x1C,0x00,
    0x00,0x00, // timeMode/reserved1  0=Disable
    0x00,0x00, // Flags
    0x00,0x00,0x00,0x00, // ecefXOrLat
    0x00,0x00,0x00,0x00, // ecefYOrLon
    0x00,0x00,0x00,0x00, // ecefZOrAlt
    0x00,0x00,0x00,0x00, // fixedPosAcc  mm
    0xE8,0x03,0x00,0x00, // svinMinDur   s  1000 samples
    0xE8,0x03,0x00,0x00, // svinAccLimit mm 1000 1m
    0xAA,0xAA };  // compute checksum

  SendUBLOX(Fifo, sizeof(ubx_cfg_tmode2), ubx_cfg_tmode2); // Computes Fletcher sums on packet, and dispatches
}

//****************************************************************************

void ConfigTMODE2SurveyIn(FIFO *Fifo) // LEA-M8F 2.20
{
  BYTE ubx_cfg_tmode2[] = { // CFG-TMODE2 (06 3D)
    0xB5,0x62,0x06,0x3D,0x1C,0x00,
    0x01,0x00, // timeMode/reserved1  1=Survey In
    0x00,0x00, // Flags
    0x00,0x00,0x00,0x00, // ecefXOrLat
    0x00,0x00,0x00,0x00, // ecefYOrLon
    0x00,0x00,0x00,0x00, // ecefZOrAlt
    0x00,0x00,0x00,0x00, // fixedPosAcc  mm
    0xE8,0x03,0x00,0x00, // svinMinDur   s  1000 samples
    0xE8,0x03,0x00,0x00, // svinAccLimit mm 1000 1m
    0x36,0x85 };

  SendUBLOX(Fifo, sizeof(ubx_cfg_tmode2), ubx_cfg_tmode2); // Computes Fletcher sums on packet, and dispatches
}

//****************************************************************************
