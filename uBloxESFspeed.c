// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p0000CbYjZ6CQK/sending-speed-type-11-to-f9rk-via-ubxesfmeas-0x10-0x02
// https://portal.u-blox.com/s/question/0D52p0000B83ZxrCQE/zedf9r-wheel-speed-input
//****************************************************************************

void ubx_esf_meas_gen_speed(uint32_t ttag, int32_t speed)
{
  int length = 12; // 3x uint32_t of payload
  uint8_t ubxcmd[8 + 12]; // sourcer32@gmail.com
  uint32_t *u = (uint32_t *)&ubxcmd[6]; // payload

  ubxcmd[0x00] = 0xB5;
  ubxcmd[0x01] = 0x62;

  ubxcmd[0x02] = 0x10; // ESF-MEAS
  ubxcmd[0x03] = 0x02;

  ubxcmd[0x04] = (uint8_t)(length % 256);
  ubxcmd[0x05] = (uint8_t)(length / 256);

	// Probably overkill, should truncate fine within the number space we're using
  if (speed & 0x80000000) speed |= 0xFF800000; // sign to bit 23

  u[0] = ttag; // Timestamp in your MCU time-line, receiver will add it's own upon reception
  u[1] = (1 << 11); // Flags/Id, One Sensor
  u[2] = (speed  & 0x00FFFFFF) | (11 << 24); // Sensor 11 : Speed
 
  SendUBLOX(hUARTGPS, ubxcmd, sizeof(ubxcmd)); // Sums and sends...
}

//****************************************************************************
