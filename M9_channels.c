// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p0000AOK91vCQD/can-neom9n-only-use-16-satellites-with-nav-update-rate-5hz
// https://portal.u-blox.com/s/question/0D52p0000CRqfqZCQR/is-the-neom9n-limited-to-16-svs-for-the-navigation-solution
// https://portal.u-blox.com/s/question/0D52p00009utV1UCAU/m9n-gnss-config-cannot-change-minmax-number-of-channels
//****************************************************************************

/*
  Default operation of u-Blox M9 parts is to drop to 16 Satellite Solution at >= 10 Hz

  There are alternate modes to select 16 or 32 Satellite Solution

    B5 62 06 8A 09 00 00 01 00 00 D5 00 11 20 00 A0 CD    0=AUTO 16/32
    B5 62 06 8A 09 00 00 01 00 00 D5 00 11 20 01 A1 CE    1=16 CHANNELS
    B5 62 06 8A 09 00 00 01 00 00 D5 00 11 20 02 A2 CF    2=32 CHANNELS

  Uses 0x201100D5 CFG-NAVSPG-0D5 key

 If this saves you several man-hours/days consider https://paypal.me/cliveone
  8-Aug-2024  sourcer32@gmail.com
*/

uint8_t M9AutoSolution[] = { // Default/Automatic >= 10 Hz 16 Satellites, else 32 Satellites
  0xB5,0x62,0x06,0x8A,0x09,0x00, // UBX-CFG-VALSET
  0x00,0x01,0x00,0x00,      // RAM
  0xD5,0x00,0x11,0x20,0x00, // 0x201100D5 CFG-NAVSPG-0D5 = 0 AUTO/DEFAULT
  0xA0,0xCD }; // Fletcher checksum, correct for preceeding frame

uint8_t M9Use16[] = {
  0xB5,0x62,0x06,0x8A,0x09,0x00, // UBX-CFG-VALSET
  0x00,0x01,0x00,0x00,      // RAM
  0xD5,0x00,0x11,0x20,0x01, // 0x201100D5 CFG-NAVSPG-0D5 = 1 16-CHANNELS
  0xA1,0xCE }; // Fletcher checksum, correct for preceeding frame

uint8_t M9Use32[] = {
  0xB5,0x62,0x06,0x8A,0x09,0x00, // UBX-CFG-VALSET
  0x00,0x01,0x00,0x00,      // RAM
  0xD5,0x00,0x11,0x20,0x02, // 0x201100D5 CFG-NAVSPG-0D5 = 2 32-CHANNELS
  0xA2,0xCF }; // Fletcher checksum, correct for preceeding frame

SerialGPS.write(M9Use32, sizeof(M9Use32)); // Arduino method

