Ramblings on uBlox OSNMA one ZED-F9P HPG 1.50 (L1/L2), Using UBX-MGA-GAL

 https://portal.u-blox.com/s/question/0D5Oj00000N0yaLKAR/f9p-150-osnma-configuration

 https://raw.githubusercontent.com/Algafix/OSNMA/master/ublox_run/OSNMA_MerkleTree.xml

 The SECP256R1 Public Key looks too short, does appear to need the leading 0x03 to accept. The UBX packet doesn't look large enough for P-521 key

 The Merkel Key has several nodes, not sure how these get communicated, but under review

 Packet sizes fixed at 36 and 72-bytes. I suspect issues with the P-521 compressed key ingress, but need a working example.
 
  If this saves you several man-hours/days consider https://paypal.me/cliveone
  
   29-Jul-2024  sourcer32@gmail.com

ENABLE OSNMA
```
UBX-CFG-VALSET, CFG-GAL-USE_OSNMA = 1 (RAM)
B5 62 06 8A 09 00 00 01 00 00 05 00 35 10 01 E5 0A
```
  *  https://content.u-blox.com/sites/default/files/documents/u-blox-F9-HPG-1.50_InterfaceDescription_UBXDOC-963802114-12815.pdf#page=245
  *  https://content.u-blox.com/sites/default/files/documents/ZED-F9P_FW100HPG150_RN_UBXDOC-963802114-12826.pdf#page=8

7 = OSNMA PUBKEY
```
// 0397EB43789AA0F6D052A638468ECF5278E6F6DF8465ECB8D8B84B8C7A3501F73B
// 03 97 EB 43 78 9A A0 F6 D0 52 A6 38 46 8E CF 52 78 E6 F6 DF 84 65 EC B8 D8 B8 4B 8C 7A 35 01 F7 3B
//  ID:1  TYPE:ECDSA P-256/SHA-256 (SHOULD BE VALID)

uint8_t  ubx_mga_gal_osnma_pubkey[] = {
  0xB5,0x62,0x13,0x02,0x48,0x00,  // 0x13,0x02 = UBX-MGA-GAL (72-Bytes)
  0x07,0x00,0x11,0x00,            // 0x07 = PUBKEY, +2 Low Nibble Key#, High Nibble 1=ECDSA P-256,3=ECDSA P-521
  0x03,0x97,0xEB,0x43,0x78,0x9A,0xA0,0xF6, // KEY
  0xD0,0x52,0xA6,0x38,0x46,0x8E,0xCF,0x52,
  0x78,0xE6,0xF6,0xDF,0x84,0x65,0xEC,0xB8,
  0xD8,0xB8,0x4B,0x8C,0x7A,0x35,0x01,0xF7,
  0x3B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,
  0xE3,0x08 }; // Fletcher Checksum
```
```
// 03007520540B6C1981A15A116BF1F312046413EBD802F751A2B8E4636F0504747BF52E06DED189E89327DF25FA61503E8A6B24C39F5BE0DB39AE676434323045ECB4DD
// 03 00 75 20 54 0B 6C 19 81 A1 5A 11 6B F1 F3 12 04 64 13 EB D8 02 F7 51 A2 B8 E4 63 6F 05 04 74 7B F5 2E 06 DE D1 89 E8 93 27 DF 25 FA 61 50 3E 8A 6B 24 C3 9F 5B E0 DB 39 AE 67 64 34 32 30 45 EC B4 DD
//  ID:8  TYPE:ECDSA P-521/SHA-512 (EXAMPLE OF FORM)

uint8_t  ubx_mga_gal_osnma_pubkey[] = {
  0xB5,0x62,0x13,0x02,0x48,0x00,  // 0x13,0x02 = UBX-MGA-GAL (72-Bytes)
  0x07,0x00,0x38,0x00,            // 0x07 = PUBKEY, +2 Low Nibble Key#, High Nibble 1=ECDSA P-256,3=ECDSA P-521
  0x03,0x00,0x75,0x20,0x54,0x0B,0x6C,0x19, // KEY
  0x81,0xA1,0x5A,0x11,0x6B,0xF1,0xF3,0x12,
  0x04,0x64,0x13,0xEB,0xD8,0x02,0xF7,0x51,
  0xA2,0xB8,0xE4,0x63,0x6F,0x05,0x04,0x74,
  0x7B,0xF5,0x2E,0x06,0xDE,0xD1,0x89,0xE8,
  0x93,0x27,0xDF,0x25,0xFA,0x61,0x50,0x3E,
  0x8A,0x6B,0x24,0xC3,0x9F,0x5B,0xE0,0xDB,
  0x39,0xAE,0x67,0x64,0x34,0x32,0x30,0x45,
  0xEC,0xB4,0xDD,0x00,
  0x21,0x75 }; // Fletcher Checksum
```
8 = OSNMA MERKEL
```
// 832E15EDE55655EAC6E399A539477B7C034CCE24C3C93FFC904ACD9BF842F04E
// 83 2E 15 ED E5 56 55 EA C6 E3 99 A5 39 47 7B 7C 03 4C CE 24 C3 C9 3F FC 90 4A CD 9B F8 42 F0 4E
//  (SHOULD BE VALID)

uint8_t  ubx_mga_gal_osnma_merkel[] = {
  0xB5,0x62,0x13,0x02,0x24,0x00,  // 0x13,0x02 = UBX-MGA-GAL (36-Bytes)
  0x08,0x00,0x00,0x00,            // 0x08 = MERKEL, +2 0=CURRENT,1=FUTURE
  0x83,0x2E,0x15,0xED,0xE5,0x56,0x55,0xEA, // KEY
  0xC6,0xE3,0x99,0xA5,0x39,0x47,0x7B,0x7C,
  0x03,0x4C,0xCE,0x24,0xC3,0xC9,0x3F,0xFC,
  0x90,0x4A,0xCD,0x9B,0xF8,0x42,0xF0,0x4E,
  0x8E,0xDC }; // Fletcher Checksum
```

I'm in the process of going through the sign-up process outlined here

https://customersupport.septentrio.com/s/article/How-to-retrieve-keys-and-insert-them-in-the-receiver-to-use-the-OSNMA-service

https://www.gsc-europa.eu/

Signing up for developer access to OSNMA Public Key and Merkel Tree data

The compressed public key leads with a 2 or 3 byte

  30-Jul-2024  sourcer32@gmail.com

  *  https://content.u-blox.com/sites/default/files/documents/ZED-F9P_FW100HPG150_RN_UBXDOC-963802114-12826.pdf
  *  https://content.u-blox.com/sites/default/files/2024-07/UBX_F9_100_HPG_150_ZED_F9P.82afa0b888dab694869a5098bf3a9f0b.bin

  *  https://content.u-blox.com/sites/default/files/documents/ZED-F9P-FW100HPG151_RN_UBXDOC-963802114-13110.pdf

  22-Nov-2024  sourcer32@gmail.com
  
  
Ok, so the secret here is to make sure that Time Trusted is set, you get one shot at this, so awkward to do manually in uCenter. If you mess up you must cold start

Go to UBX-MGA-INI-TIME_UTC in Message View

Set Accuracy to 5 seconds, Check "Time Trusted" box, must do both.

Adjust Time HH:MM:SS to UTC, not your local time, here in Chicago had to advance 5 hours for Central/Day Lights Saving.

Adjust the MM:SS to something close to the time on your phone, in the future.

When you hit that time, click the SEND button to dispatch.

You can check the result via UBX-NAV-TIMETRUSTED, and how slow your reaction time is. ~500 ms in my case.

On an embedded platform this can be done more mechanically if you have a button to confirm UTC as determine via GPS/GNSS at a location/time you trust the system.

I'll perhaps add screen shots here later. But check  https://portal.u-blox.com/s/question/0D5Oj00000N0yaLKAR/f9p-150-osnma-configuration

  31-Jul-2024  sourcer32@gmail.com

##  SETUP METHOD
###  ENABLE OSNMA
```
UBX-CFB-VALSET, CFG-GAL-USE_OSNMA = 1 (RAM)
B5 62 06 8A 09 00 00 01 00 00 05 00 35 10 01 E5 0A
```

###  SET MERKEL TABLE ROOT
UBX-MGA-GAL-OSNMA MERKEL - CURRENT
```
83 2E 15 ED E5 56 55 EA C6 E3 99 A5 39 47 7B 7C 03 4C CE 24 C3 C9 3F FC 90 4A CD 9B F8 42 F0 4E
```
![alt text](ubx_mga_gal_osnma_merkel_001.jpg?raw=true)

###  SET PUBLIC KEY
UBX-MGA-GAL-OSNMA PUBKEY - ID:1  TYPE:ECDSA P-256/SHA-256
```
03 97 EB 43 78 9A A0 F6 D0 52 A6 38 46 8E CF 52 78 E6 F6 DF 84 65 EC B8 D8 B8 4B 8C 7A 35 01 F7 3B
```
![alt text](ubx_mga_gal_osnma_pubkey_001.jpg?raw=true)

### SET TIME TRUSTED
Set Accuracy to 5 seconds, Check "Time Trusted" box, must do both.

Adjust Time HH:MM:SS to UTC, not your local time, here in Chicago had to advance 5 hours for Central/Day Lights Saving.

Adjust the MM:SS to something close to the time on your phone, in the future.

When you hit that time, click the SEND button to dispatch.
![alt text](ubx_mga_ini_timeutc_001.jpg?raw=true)

### CHECK TIME TRUSTED
![alt text](ubx_nav_trustedtime_001.jpg?raw=true)

### CHECK OSNMA ENABLED, KEY / TABLE ACCEPTANCE
![alt text](osnma_test_001.jpg?raw=true)

### CHECK OSNMA PROCESSING / TESLA KEY / AUTHENTICATION
![alt text](osnma_test_002.jpg?raw=true)

  If this saves you several man-hours/days consider https://paypal.me/cliveone

  31-Jul-2024  sourcer32@gmail.com
