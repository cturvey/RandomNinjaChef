/* UBX-NAV-PVAT (01 17) and NAV2-PVAT (29 17)
+0 U4 iTow
+4 U1 version
+5 X1 valid
 0 validDate
 1 validTime
 2 fullyResolved
 3 validMag
+6 U2 year
+8 U1 month
+9 U1 day
+10 U1 hour
+11 U1 min
+12 U1 second
+13 U1[1] reserved0
+14 U1[1] reserved1
+16 U4 tAcc
+20 I4 nano
+24 U1 fixType
+25 X1 flags
 0 gnssFixOk
 1 diffSoln
 3 vehRollValid
 4 vehPitchValid
 5 vehHeadingValid
 6..7 carrSoln
+26 X1 flags2
 5 confirmedAvai
 6 confirmedDate
 7 configmedTime
+27 U1 numSv
+28 I4 lon 1e-7
+32 I4 lat
+36 I4 height mm
+40 I4 hMSL
+44 U4 hAcc mm
+48 U4 vAcc
+52 I4 velN mm/s
+56 I4 velE
+60 I4 velD
+64 I4 gSpeed
+68 U4 sAcc
+72 I4 vehRoll 1e-5 degree
+76 I4 vehPitch
+80 I4 vehHeading
+84 I4 motHeading
+88 U2 accRoll 1e-2 degree
+90 U2 accPitch
+92 U2 accHeading
+94 I2 magDec 1e-2 degree
+96 U2 magAcc
+98 U2 errEllipseOrient 1e-2 degree
100 U4 errEllipseMajor mm
104 U4 errEllipseMinor
108 U1[4] reserved2
112 U1[4] reserved3

116 total
*/
