For "Secure" parts where configuration is implicitly disabled, and you can't access the session keys in more normative ways. ie (0x10F60009) CFG-SEC-CFG_LOCK = 1

The SessionID is 24-bytes and allows for unique salting of signed SessionID + SHA256 (56-Bytes) designed to thwart replay attacks when using UBX-SEC-ECSIGN (0x27 0x04)

Can be changed on-the-fly so subsequent messages will use the new settings, might take a message for the transition to occur.

The availability of UBX-SEC-SESSID is reported in several public release notes, the form here has been derived via inspection and testing.
```
UBX-SEC-SESSID (0x27 0x05) 28-Bytes
Read (Query - Zero payload) and Write (28-Byte payload)

+0 B  0x01 Version#1
+1 B  0x00 Reserved#1
+2 W  0x00 Reserved#2
+4 B[24] Session ID 24-bytes

Operates on RAM copy of CFG-SEC-ECCFGSESSIONID[0,1,2] 3x QUAD-WORDS
```
```
Configuration Keys for the ECSIGN Session ID (Default)
    0x50F60006 CFG-SEC-ECCFGSESSIONID0              0000000000000000
    0x50F60007 CFG-SEC-ECCFGSESSIONID1              0000000000000000
    0x50F60008 CFG-SEC-ECCFGSESSIONID2              0000000000000000
```

sourcer32@gmail.com  15-June-2026

If this saves you several man-hours/days consider 
  *  https://paypal.me/cliveone
  *  https://www.venmo.com/u/cliveone
  *  https://buymeacoffee.com/cliveone
