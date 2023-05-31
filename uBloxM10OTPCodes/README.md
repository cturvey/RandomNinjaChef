I'm using this as a parking place for useful codes. If this helps you, contribute via https://paypal.me/cliveone

Setting OTP values in u-Blox M10 devices for permanent configuration. Use sparingly its a finite resource. OTP (One Time Programmable), really its a once and done, use with thought and consideration...

Related ramblings

https://portal.u-blox.com/s/question/0D52p0000DbCR9CCQW/change-default-baudrate-on-m10-gps

https://portal.u-blox.com/s/question/0D52p0000DgwUFOCQ2/how-do-i-store-configuration-item-in-the-flash-memory-of-an-m10s-i-only-get-the-red-triangle-in-the-ucenter-software


  * CFG-ITFM-ENABLE=1,CFG-RATE-MEAS=200
    *  B5 62 06 41 17 00 04 01 A4 0B 66 95 58 93 28 EF 12 05 0D 00 41 10 01 01 00 21 30 C8 00 9F EA

  * CFG-UART1-BAUDRATE=115200,CFG-ITFM-ENABLE=1,CFG-RATE-MEAS=200
    *  B5 62 06 41 1F 00 04 01 A4 13 1F 2E 32 FA 28 EF 12 05 01 00 52 40 00 C2 01 00 0D 00 41 10 01 01 00 21 30 C8 00 98 2F

  * 57600 Baud, Permanent Default
    *  B5 62 06 41 14 00 04 01 A4 08 23 FF 4D 7E 28 EF 12 05 01 00 52 40 00 E1 00 00 9B 2C

  * 115200 Baud, Permanent Default
    *  B5 62 06 41 14 00 04 01 A4 08 DB 36 5D 5D 28 EF 12 05 01 00 52 40 00 C2 01 00 5B BD
 
   * 192 MHz CPU clock mode 
     *  B5 62 06 41 10 00 03 00 04 1F 54 5E 79 BF 28 EF 12 05 FD FF FF FF 8F 0D 
     *  B5 62 06 41 1C 00 04 01 A4 10 BD 34 F9 12 28 EF 12 05 05 00 A4 40 00 B0 71 0B 0A 00 A4 40 00 D8 B8 05 DE AE
     *  https://content.u-blox.com/sites/default/files/documents/MIA-M10Q_IntegrationManual_UBX-21028173.pdf
   * Internal LNA Mode, Bypass
     *  B5 62 06 41 10 00 03 00 05 1F 79 B2 0A E5 28 EF 12 05 9F FF FF FF 62 FB
     *  https://content.u-blox.com/sites/default/files/documents/MIA-M10Q_IntegrationManual_UBX-21028173.pdf

31-May-2022  sourcer32@gmail.com 
