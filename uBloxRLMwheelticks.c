// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p0000B83ZxrCQE/zedf9r-wheel-speed-input
//****************************************************************************
//
// Quick demo of sensor measurement push into the ZED-F9R (F9K,M8U,M8L, etc)
//
// The RLM (Robot Lawn Mower) requires wheel tick information for both rear
// wheels, with direction. The pins aren't available to acheive this, so
// the host needs to push in values. Basically the change (positive/negative)
// within the last measurement epoch, not an accumulation.
//
// The receiver will echo back a time-stamped version via enabled UBX-ESF-MEAS
// so you can use that as a verification it got the message, and also acheive
// time-transfer within your system.
//
// If this saves you several man-hours/days consider https://paypal.me/cliveone
//
// 27-Oct-2021  sourcer32@gmail.com
//
// uBlox reached out to indicate negative values here use a sign/magnitude format
//  I'm waiting to see documentation, the Accel/Gyro use 2's complement
//
// 23-Mar-2023  sourcer32@gmail.com
//
//****************************************************************************

void ubx_esf_meas_gen(uint32_t ttag, int32_t left_rear, int32_t right_rear)
{
  int length = 16;
  uint8_t ubxcmd[8 + 16]; // sourcer32@gmail.com
  uint32_t *u = (uint32_t *)&ubxcmd[6]; // payload

  ubxcmd[0x00] = 0xB5;
  ubxcmd[0x01] = 0x62;

  ubxcmd[0x02] = 0x10; // ESF-MEAS
  ubxcmd[0x03] = 0x02;

  ubxcmd[0x04] = (uint8_t)(length % 256);
  ubxcmd[0x05] = (uint8_t)(length / 256);

#if 0
  // Probably overkill, should truncate fine within the number space we're using
  if (left_rear  & 0x80000000) left_rear  |= 0xFF800000; // sign to bit 23
  if (right_rear & 0x80000000) right_rear |= 0xFF800000; //  receiver uses int24_t
#else
  // Been suggested that this needs to be a sign/magnitude format
  if (left_rear  & 0x80000000) left_rear  = -left_rear  | 0x800000; // sign to bit 23,
  if (right_rear & 0x80000000) right_rear = -right_rear | 0x800000; //  positive tick count 0..22
#endif

  u[0] = ttag; // Timestamp in your MCU time-line, receiver will add it's own upon reception
  u[1] = (2 << 11); // Flags/Id, Two Sensors
  u[2] = (left_rear  & 0x00FFFFFF) | (8 << 24); // Sensor 8 : Left-Rear Wheel Tick
  u[3] = (right_rear & 0x00FFFFFF) | (9 << 24); // Sensor 9 : Right-Rear Wheel Tick

  SendUBLOX(hUARTGPS, ubxcmd, sizeof(ubxcmd)); // Sums and sends...
}

//****************************************************************************
