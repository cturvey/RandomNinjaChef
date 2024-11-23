## uBlox MEASX, MEASD12, MEASC12, MEAS20, MEAS50 Resources

If this saves you several man-hours/days consider https://paypal.me/cliveone

Related to
  *  https://portal.u-blox.com/s/question/0D5Oj00000ZTxtPKAT/ubxrxmmeas50-payload-format
  *  https://portal.u-blox.com/s/question/0D52p0000DXaY0oCQF/does-the-m10-firmware-have-a-commandmessage-that-reports-the-raw-pseudoranges-or-raw-code-phases-i-can-see-the-m8-and-m9-both-report-these-but-is-absent-in-the-m10-interface-description
  *  https://portal.u-blox.com/s/question/0D5Oj00000NjJURKA3/cloudlocate-what-are-measc12measd12-for
  *  https://portal.u-blox.com/s/question/0D52p0000DWDqq3CQD/rtcm-messages
  *  https://developer.thingstream.io/guides/location-services/cloudlocate-new-getting-started
  *  https://developer.thingstream.io/guides/location-services/cloudlocate-new-getting-started/using-gnss-raw-measurements/using_cloudlocate_with_m8
  *  https://portal.thingstream.io/app/downloads


23-Nov-2024 sourcer32@gmail.com


```
---UBX-----------------------------------------------------------------------
02 80 RXM-MEASD12     - 35 A9 : 35 A9 20
0000 : 03 63 81 17 C8 EE 56 54-48 DB E2 44             .c....VTH..D

tWICgAwAA2OBF8juVlRI2+JENak=

000000 ?? ONLY GPS
11011 svId+1 (doppler reference)
00011 100000010001 svId+1 doppDelta*2.5
01111 100100011101
11001 010110010101
00010 010001101101
11110 001001000100

 0

#0  28
#1   4 2065 5162.5
#2  16 2333 5832.5
#3  26 1429 3572.5
#4   3 1133 2832.5
#5  31  580 1450.0
index:96
-----------------------------------------------------------------------------
```
  *  https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxMEAS/measd12.c

```
---UBX-----------------------------------------------------------------------
02 84 RXM-MEAS20      - 1C 04 : 1C 04 28
0000 : 36 1A E8 3C CB 08 17 9A-0B C9 67 52 15 64 56 36 6..<......gR.dV6
0010 : 68 FE E1 B1                                     h...

tWIChBQANhroPMsIF5oLyWdSFWRWNmj+4bEcBA==

00 gnssId
11011 0000110101110100 svId+1 chips/64.0 (doppler reference)
00011 1100110010110000 10000001 svId+1 chips/64.0 doppDelta*40.0
01111 0011010000010111 10010010
11001 1101010010000101 01011001
00010 1011000110110011 01000111
11110 1110000110110001

0:GPS
# 0 28  3444   53.813
# 1  4 52400  818.750  129  5160
# 2 16 13335  208.359  146  5840
# 3 26 54405  850.078   89  3560
# 4  3 45491  710.797   71  2840
# 5 31 57777  902.766
index:160
-----------------------------------------------------------------------------
```
  *  https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxMEAS/meas20.c


```
---UBX-----------------------------------------------------------------------
02 86 RXM-MEAS50      - 6A 84 : 6A 84 58
0000 : 18 38 87 01 E6 58 33 54-3C D0 5F C4 A3 3A 90 A9 .8...X3T<._..:..
0010 : BA 1B 0D 74 96 00 F7 0D-89 65 47 ED 96 C3 66 05 ...t.....eG...f.
0020 : 63 66 1C E3 3C AE 37 D6-D6 AF 09 0B A3 A9 C4 62 cf..<.7........b
0030 : C6 68                                           .h

tWIChjIAGDiHAeZYM1Q80F/EozqQqbobDXSWAPcNiWVH7ZbDZgVjZhzjPK431tavCQujqcRixmhqhA==

# 0 0:GPS 25  28942  226.109
# 1 0:GPS  4 104800  818.750 3285 6570
# 2 0:GPS 16  26671  208.367 3621 7242
# 3 0:GPS 26 108810  850.078 2490 4980
# 4 0:GPS 28   6889   53.820  704 1408
# 5 0:GPS 31 115554  902.766 1429 2858
# 6 0:GPS 32  93366  729.422  435  870
# 7 0:GPS  3  90982  710.797
# 8 0:GPS 29 116345  908.945
# 9 1:GAL 29  57179  446.711
#10 1:GAL 27  96328  752.563
#11 1:GAL 30  15004  117.219
#12 1:GAL  7  22733  177.602
index:397 0
-----------------------------------------------------------------------------
```
  *  https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxMEAS/meas50.c

    
##  Support
 
  *  https://paypal.me/cliveone
  *  https://www.venmo.com/u/cliveone
  *  https://buymeacoffee.com/cliveone
  *  Amazon or Starbucks Gift Cards
  *  Google Pay (Probably)
