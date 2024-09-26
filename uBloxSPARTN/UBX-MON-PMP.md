### UBX-MON-PMP

Discussed with details of packet as I've disected it around post 29

https://portal.u-blox.com/s/question/0D52p0000DLWEeLCQX/lband-spartn-corrections-via-d9s-and-f9p-correct-d9s-configuration

```
0x0A 0x35 UBX-MON-PMP (Reports Signal Strength / Frequency) 24-byte Payload (4-byte header, 20-byte entries)
 
+0x00 U1 version
+0x01 U1 nEntries (1)
+0x02 U1[2] reserved0
 
Repeated Entries (typically 1) 20-byte(s) each
+0x00 U4 reserved1
   [U4 timeTag ms]
+0x04 U4 status, Bit 0 locked, Bit 1 frameSync
+0x08 U1[8] reserved2
   [U4 lockTime ms]
   [U4 centerFrequency Hz]
+0x10 U1 cno dBHz     fixed point 8.8 representation
+0x11 U1 cnoFrac dBHz
+0x12 U1[2] reserved3
```
```
---UBX-----------------------------------------------------------------------
0000 : B5 62 0A 35 18 00 00 01-00 00 F0 C5 1C 14 03 00 .b.5............
0010 : 00 00 C0 E2 F8 0E EA 1A-C3 5C 32 35 00 00 72 C0 .........\25..r.

0A 35 MON-PMP         - 72 C0 : 72 C0 32
MON-PMP
0000 : 00 01 00 00 F0 C5 1C 14-03 00 00 00 C0 E2 F8 0E ................
0010 : EA 1A C3 5C 32 35 00 00-                        ...\25..

 337430000  251192000 1556290282      13618
CNO: 50.21
```
```
0x20910322  CFG-MSGOUT-UBX_MON_PMP_I2C
0x20910323  CFG-MSGOUT-UBX_MON_PMP_UART1
0x20910324  CFG-MSGOUT-UBX_MON_PMP_UART2
0x20910325  CFG-MSGOUT-UBX_MON_PMP_USB
0x20910326  CFG-MSGOUT-UBX_MON_PMP_SPI
```
https://content.u-blox.com/sites/default/files/C101-D9S-AppBoard_UserGuide_UBX-20031865.pdf#page=8 (disclosure)

47.55 dBHz (13.625 Eb/NO [dB]), no demodulation errors
![alt text](neo_d9s_004.jpg?raw=true)
