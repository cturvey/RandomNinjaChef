/*
  SOUTHPAN SBAS

  https://portal.u-blox.com/s/question/0D5Oj000004z5tdKAA/how-do-i-make-zedf9p-work-with-southpan-sbas-service
  https://portal.u-blox.com/s/question/0D5Oj00000PXhOFKA1/does-the-maxm10s-support-southpan-sbas-corrections

m8_southpan_sbas.txt

CFG-SBAS - 06 16 08 00 03 02 01 00 04 00 00 00

 If this saves you several man-hours/days consider https://paypal.me/cliveone
  4-Nov-2024  sourcer32@gmail.com
*/

uint8_t ubx8_cfg_sbas[] = { // Enable SBAS, One Search, Custom 122
  0xB5,0x62,0x06,0x16,0x08,0x00,
  0x03,0x02,0x01,0x00,0x04,0x00,0x00,0x00,
  0x2E,0xC6 };

