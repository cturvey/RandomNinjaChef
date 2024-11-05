# SOUTHPAN SBAS on PRN122 for M9/F9/M10/F10

If you found this useful, or it saved you many hours of work, consider contributing to my efforts
  *  https://paypal.me/cliveone  
  *  https://www.venmo.com/u/cliveone
  *  https://buymeacoffee.com/cliveone

### Suggested Reading..

  *  https://portal.u-blox.com/s/question/0D5Oj000004z5tdKAA/how-do-i-make-zedf9p-work-with-southpan-sbas-service
  *  https://portal.u-blox.com/s/question/0D5Oj00000PXhOFKA1/does-the-maxm10s-support-southpan-sbas-corrections
  *  https://frontiersi.github.io/southpan-receiver-docs/docs/configuration/ublox.html
  *  https://frontiersi.github.io/southpan-receiver-docs/docs/configuration/ublox.html#generation-9-modules
  *  https://frontiersi.github.io/southpan-receiver-docs/docs/configuration/ublox.html#generation-10-modules

### Receivers
  *  https://www.beitian.com/en/sys-pd/832.html  (BE-126)
  *  https://www.beitian.com/en/sys-pd/524.html  (BE-122)

### Generation 9 Configuration File
![alt text](cfg9_southpan_sbas.jpg?raw=true)
  *  [m9_southpan_sbas.txt](https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxSouthPanSBAS/m9_southpan_sbas.txt)
```
# Config changes format version 1.0
# created by u-center version 23.08 at 13:08:12 on Monday, 04 Nov 2024
[del]
[set]
  RAM CFG-SIGNAL-SBAS_ENA  1                    # write value 1                    to item id 0x10310020 in layer 0
  RAM CFG-SIGNAL-SBAS_L1CA_ENA 1                # write value 1                    to item id 0x10310005 in layer 0
  RAM CFG-SBAS-USE_TESTMODE 1                   # write value 1                    to item id 0x10360002 in layer 0
  RAM CFG-SBAS-USE_RANGING 0                    # write value 0                    to item id 0x10360003 in layer 0
  RAM CFG-SBAS-USE_DIFFCORR 1                   # write value 1                    to item id 0x10360004 in layer 0
  RAM CFG-SBAS-USE_INTEGRITY 0                  # write value 0                    to item id 0x10360005 in layer 0
  RAM CFG-SBAS-PRNSCANMASK 0x4                  # write value 4  0x4               to item id 0x50360006 in layer 0
```
### C code for M9/F9/M10/F10
Flash only works with modules with FLASH, which doesn't include MAX-M10, MIA-M10
```
uint8_t ubx9_cfg_valset_sbas122_ram[] = {
  0xB5,0x62,0x06,0x8A,0x2E,0x00,
  0x00,0x01,0x00,0x00,  // RAM
  0x20,0x00,0x31,0x10,0x01, // CFG-SIGNAL-SBAS_ENA=1
  0x05,0x00,0x31,0x10,0x01, // CFG-SIGNAL-SBAS_L1CA_ENA=1
  0x02,0x00,0x36,0x10,0x01, // CFG-SBAS-USE_TESTMODE=1
  0x03,0x00,0x36,0x10,0x00, // CFG-SBAS-USE_RANGING=0
  0x04,0x00,0x36,0x10,0x01, // CFG-SBAS-USE_DIFFCORR=1
  0x05,0x00,0x36,0x10,0x00, // CFG-SBAS-USE_INTEGRITY=0
  0x06,0x00,0x36,0x50,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // CFG-SBAS-PRNSCANMASK=4 (PRN120+BIT2 == PRN122)
  0x20,0xAA };

uint8_t ubx9_cfg_valset_sbas122_ram_bbr[] = {
  0xB5,0x62,0x06,0x8A,0x2E,0x00,
  0x00,0x03,0x00,0x00,  // RAM+BBR
  0x20,0x00,0x31,0x10,0x01,
  0x05,0x00,0x31,0x10,0x01,
  0x02,0x00,0x36,0x10,0x01,
  0x03,0x00,0x36,0x10,0x00,
  0x04,0x00,0x36,0x10,0x01,
  0x05,0x00,0x36,0x10,0x00,
  0x06,0x00,0x36,0x50,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x22,0x04 };

uint8_t ubx9_cfg_valset_sbas122_ram_bbr_flash[] = {
  0xB5,0x62,0x06,0x8A,0x2E,0x00,
  0x00,0x07,0x00,0x00,  // RAM+BBR+FLASH
  0x20,0x00,0x31,0x10,0x01,
  0x05,0x00,0x31,0x10,0x01,
  0x02,0x00,0x36,0x10,0x01,
  0x03,0x00,0x36,0x10,0x00,
  0x04,0x00,0x36,0x10,0x01,
  0x05,0x00,0x36,0x10,0x00,
  0x06,0x00,0x36,0x50,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x26,0xB8 };
```
### Generation 8 Configuration File
  *  [m8_southpan_sbas.txt](https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxSouthPanSBAS/m8_southpan_sbas.txt)
```
CFG-SBAS - 06 16 08 00 03 02 01 00 04 00 00 00
```
### C code for M8
```
uint8_t ubx8_cfg_sbas[] = { // Enable SBAS, One Search, Custom 122
  0xB5,0x62,0x06,0x16,0x08,0x00,
  0x03,0x02,0x01,0x00,0x04,0x00,0x00,0x00,
  0x2E,0xC6 };
```
