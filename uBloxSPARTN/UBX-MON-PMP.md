### UBX-MON-PMP

Discussed with details of packet as I've disected it around post 29


https://portal.u-blox.com/s/question/0D52p0000DLWEeLCQX/lband-spartn-corrections-via-d9s-and-f9p-correct-d9s-configuration

```
0x0A 0x35 UBX-MON-PMP (Reports Signal Strength / Frequency) 24-byte Payload (4-byte header, 20-byte entries)
 
+0x00 U1 version
+0x01 U1 nEntries (1)
+0x02 U1[2] reserved0
 
Repeated Entries (typically 1)
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

https://content.u-blox.com/sites/default/files/C101-D9S-AppBoard_UserGuide_UBX-20031865.pdf (disclosure)
