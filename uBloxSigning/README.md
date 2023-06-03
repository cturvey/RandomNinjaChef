Place holder for UBX-SEC-SIGN and UBX-SEC-ECSIGN examples

UBX-SEC-SIGN uses a salted-SHA-256 using a fixed, and dynamic seed front and back of specific messages. The message contains the class/id of the messaged being signed, and the fletcher checksum of the packet.

https://portal.u-blox.com/s/question/0D52p00008HKCkJCAX/how-to-verify-ubxsecsign

UBX-SEC-ECSIGN uses a SHA-256 to hash the transmission on a specific interface, less the UBX-SEC-ECSIGN message itself. You can verify the SHA-256 hash without knowing the signing keys. The hash and session id are signed using SECP192R1 Elliptic Curve with a private key, and you can authenticate the message using the public key. The primary vunerability here is if you know, or can recover, the private key. The presumed use case is that the private key is burned into the receiver's OTP during a vendor's production programming/test and not shared with the application side. The application only needs the public key, and can thwart replay attacks by using, and checking, a unique session id.

The vendor should probably generate unique keys to bind a specific ZED-F9x or NEO-M9x to a specific board, although this may hamper the interchangability of parts/subsystems.

```
---UBX-----------------------------------------------------------------------
27 04 SEC-ECSIGN      - 3A BD : 3A BD 116
UBX-SEC-ECSIGN
0000 : 01 00 16 00 25 28 52 FA-92 E9 C6 C2 DC 1D B4 DF ....%(R.........
0010 : FB 8B 0B F8 CE 33 EB 87-60 EF 95 D2 B0 5B 7A F2 .....3..`....[z.
0020 : E0 8E A9 27 00 00 00 00-00 00 00 00 00 00 00 00 ...'............
0030 : 00 00 00 00 00 00 00 00-00 00 00 00 43 58 8C CB ............CX..
0040 : 06 44 C0 62 46 FD 4C 47-D8 B1 C3 62 49 48 EE 3E .D.bF.LG...bIH.>
0050 : C7 E0 9F A2 64 A2 47 E9-E4 D8 DC 74 17 6C 90 4B ....d.G....t.l.K
0060 : 9D D0 76 95 A5 F5 AC 50-38 87 BA 4F             ..v....P8..O

Packets : 22
 252852fa92e9c6c2dc1db4dffb8b0bf8ce33eb8760ef95d2b05b7af2e08ea927
 pass
 8f945cdaf783fb218be375282b2d9b75eaa8faea47adcbc915d32611b31181a9
 9a477acb44927a888be375282b2d9b75eaa8faea47adcbc9
 verified
-----------------------------------------------------------------------------
 
Payload
 +0x00  Version#1
 +0x02  Packet Count
 +0x04  SHA256 (32-bytes)
 +0x24  Session ID (24-bytes)
 +0x3C  SECP192R1 Signing of SHA256+Session ID (48-bytes)
```

```
ECDSA Example

// Input (I):  9A 47 7A CB 44 92 7A 88 8B E3 75 28 2B 2D 9B 75 EA A8 FA EA 47 AD CB C9
uint8_t I[] = { 0x9A,0x47,0x7A,0xCB,0x44,0x92,0x7A,0x88,0x8B,0xE3,0x75,0x28,0x2B,0x2D,0x9B,0x75,0xEA,0xA8,0xFA,0xEA,0x47,0xAD,0xCB,0xC9 };

// Public (Px):  0F F6 26 5F 72 20 8B 39 E7 25 EB E2 8E 26 25 F3 56 17 EE FC 8A C8 66 25
uint8_t Px[] = { 0x0F,0xF6,0x26,0x5F,0x72,0x20,0x8B,0x39,0xE7,0x25,0xEB,0xE2,0x8E,0x26,0x25,0xF3,0x56,0x17,0xEE,0xFC,0x8A,0xC8,0x66,0x25 };
// Public (Py):  35 2D C7 6D F0 F3 28 34 4C 09 62 B3 0D 20 A1 97 FE EB 20 02 B4 00 11 1A
uint8_t Py[] = { 0x35,0x2D,0xC7,0x6D,0xF0,0xF3,0x28,0x34,0x4C,0x09,0x62,0xB3,0x0D,0x20,0xA1,0x97,0xFE,0xEB,0x20,0x02,0xB4,0x00,0x11,0x1A };

// Signature Part 1 (R):  43 58 8C CB 06 44 C0 62 46 FD 4C 47 D8 B1 C3 62 49 48 EE 3E C7 E0 9F A2
uint8_t R[] = { 0x43,0x58,0x8C,0xCB,0x06,0x44,0xC0,0x62,0x46,0xFD,0x4C,0x47,0xD8,0xB1,0xC3,0x62,0x49,0x48,0xEE,0x3E,0xC7,0xE0,0x9F,0xA2 };
// Signature Part 2 (S):  64 A2 47 E9 E4 D8 DC 74 17 6C 90 4B 9D D0 76 95 A5 F5 AC 50 38 87 BA 4F
uint8_t S[] = { 0x64,0xA2,0x47,0xE9,0xE4,0xD8,0xDC,0x74,0x17,0x6C,0x90,0x4B,0x9D,0xD0,0x76,0x95,0xA5,0xF5,0xAC,0x50,0x38,0x87,0xBA,0x4F };

```

sourcer32@gmail.com  3-June-2023

If this saves you several man-hours/days consider https://paypal.me/cliveone
