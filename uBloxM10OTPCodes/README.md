I'm using this as a parking place for useful codes. If this helps you, contribute via PayPal or Venmo. Submit new/compounded requests in the form of payment. Suggested $20-25 USD (Higher in California)

  *  https://paypal.me/cliveone  
  *  https://www.venmo.com/u/cliveone

Now also accepting coffee https://buymeacoffee.com/cliveone

Setting OTP values in u-Blox M10 devices for permanent configuration. Use sparingly its a finite resource. OTP (One Time Programmable), really its a once and done, use with thought and consideration...

Related ramblings

https://portal.u-blox.com/s/question/0D52p0000DbCR9CCQW/change-default-baudrate-on-m10-gps

https://portal.u-blox.com/s/question/0D52p0000DgwUFOCQ2/how-do-i-store-configuration-item-in-the-flash-memory-of-an-m10s-i-only-get-the-red-triangle-in-the-ucenter-software


  * CFG-ITFM-ENABLE=1,CFG-RATE-MEAS=200 (5Hz)
    *  B5 62 06 41 17 00 04 01 A4 0B 66 95 58 93 28 EF 12 05 0D 00 41 10 01 01 00 21 30 C8 00 9F EA

  * CFG-UART1-BAUDRATE=115200,CFG-ITFM-ENABLE=1,CFG-RATE-MEAS=200(5Hz)
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
     *  http://www.u-blox.com/docs/UBX-21028173

   * Internal LNA Mode, Low Gain
     *  B5 62 06 41 10 00 03 00 05 1F 44 EA 1E 7E 28 EF 12 05 DF FF FF FF 52 10
     *  http://www.u-blox.com/docs/UBX-22038241

   * I2C/UART SWAP  TX/SCL  RX/SDA
     *  B5 62 06 41 10 00 03 00 05 1F C2 FA FA 94 28 EF 12 05 F3 FF FF FF E6 56

   * RMC ONLY, CFG-MSGOUT-UART1 VTG=0,GGA=0,GSA=0,GSV=0,GLL=0
     *  B5 62 06 41 25 00 04 01 A4 19 6A 68 2A BB 28 EF 12 05 B1 00 91 20 00 BB 00 91 20 00 C0 00 91 20 00 C5 00 91 20 00 CA 00 91 20 00 43 0F

   * CFG-MSGOUT-UART1 GST=1
     *  B5 62 06 41 11 00 04 01 A4 05 5F 05 B8 DF 28 EF 12 05 D4 00 91 20 01 B5 4C

  * CFG-UART1-BAUDRATE=115200,CFG-RATE-MEAS=100(10Hz),NAVSPG-DYNMODEL=4(AUTOMOT),CFG-ITFM-ENABLE=1
     *  B5 62 06 41 24 00 04 01 A4 18 9E 21 E3 84 28 EF 12 05 01 00 52 40 00 C2 01 00 01 00 21 30 64 00 21 00 11 20 04 0D 00 41 10 01 41 BE

  * CFG-UART1-BAUDRATE=115200,CFG-RATE-MEAS=100(10Hz),NAVSPG-DYNMODEL=6(AIR1G/80KM),CFG-ITFM-ENABLE=1
     *  B5 62 06 41 24 00 04 01 A4 18 95 80 2B C9 28 EF 12 05 01 00 52 40 00 C2 01 00 01 00 21 30 64 00 21 00 11 20 06 0D 00 41 10 01 26 6C

18-July-2024  sourcer32@gmail.com 
