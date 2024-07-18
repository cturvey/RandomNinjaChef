  I'm using this as a parking place for useful codes. If this helps you, contribute via PayPal or Venmo. Submit new/compounded requests in the form of payment. Suggested $20-25 USD (Higher in California)

  *  https://paypal.me/cliveone  
  *  https://www.venmo.com/u/cliveone

Now also accepting coffee https://buymeacoffee.com/cliveone

Setting OTP values in u-Blox F10 devices for permanent configuration. Use sparingly its a finite resource. OTP (One Time Programmable), really its a once and done, use with thought and consideration...

Related ramblings

https://portal.u-blox.com/s/question/0D5Oj00000LvXlkKAF/does-ubxlib-support-gnss-module-neof10n

  * 57600 Baud, Permanent Default
    *  B5 62 06 41 14 00 04 01 A4 08 23 FF 4D 7E 28 EF 12 05 01 00 52 40 00 E1 00 00 9B 2C
    *  B5 62 06 41 0D 00 04 01 30 01 14 22 DF 81 28 EF 12 05 F9 47 33  << USE THIS

  * 115200 Baud, Permanent Default
    *  B5 62 06 41 14 00 04 01 A4 08 DB 36 5D 5D 28 EF 12 05 01 00 52 40 00 C2 01 00 5B BD
    *  B5 62 06 41 0D 00 04 01 30 01 82 12 D8 F6 28 EF 12 05 F8 12 1D  << USE THIS

  * Internal LNA Mode - Low Gain
    *  B5 62 06 8A 09 00 01 04 00 00 57 00 A3 20 01 B9 2F (20A30057 CFG-HW-RF_LNA_MODE=1)
    *  https://content.u-blox.com/sites/default/files/documents/NEO-F10N_IntegrationManual_UBXDOC-963802114-12193.pdf

  * Internal LNA Mode - Bypass
    * B5 62 06 8A 09 00 01 04 00 00 57 00 A3 20 02 BA 30 (20A30057 CFG-HW-RF_LNA_MODE=2)
      
17-July-2024  sourcer32@gmail.com 
