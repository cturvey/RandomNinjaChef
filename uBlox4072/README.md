# RTCM3 4072.x Proprietary Messages

### Support

If you found this useful, or it saved you many hours of work, consider contributing to my efforts
  *  https://paypal.me/cliveone  
  *  https://www.venmo.com/u/cliveone
  *  https://buymeacoffee.com/cliveone

### Suggested Reading..
  * https://portal.u-blox.com/s/question/0D52p00008z2t7QCAQ/meaning-and-need-of-ublox-rtcm-4072-message
  * https://portal.u-blox.com/s/question/0D52p00008HKDWQCA5/decoding-rtcm3-message

### 4072

Used for Moving-Base / Attitude-Orientation (NEO-M8P / ZED-F9P / ZED-F9H) and Differential Timing (ZED-F9T) applications

The 1005 Message can provide a Static ECEF Position, but lacks any Time-Domain data to establish when it was at the location. Assumed not to move,
and message is constant and repetitive, so can be lower bandwidth and the receiver can cache Station ID vs Position to determine closest Base,
or code for VRS when receiving data from multiple Base Stations. For example a Static Rover could expand radio coverage for other Rover(s)

Moving Base can either be multiple units on a single vehicle, or be used in a mobile base, like a work truck, or runway/aircraft carrier for a UAV
to land on, or approach, in a return-to-home sense.

* 4072.0 Reference Station PVT
  *  Two forms the older NEO-M8P had a form containing PVT and Interband Timing data, the newer just the PVT
  *  Provides current ECEF position, and XYZ velocity, and probably acceleration / covariance matrix
* 4072.1 Reference Station Timing
   *  Inter-Band / Inter-Constellation timing relationships at the Receiver Front-End or Base-Band
   
### Example from ZED-X20P (Triple Band)
```
-----------------------------------------------------------------------------
SIZE:084  132
0000 : D3 00 7E FE 80 00 01 00-00 12 4F 68 63 C0 00 07 ..~.......Ohc...
0010 : C5 10 16 DA 78 8B 9F 36-93 C0 C5 83 EB FF FF E0 ....x..6........
0020 : 00 00 12 00 00 5C 00 00-00 00 00 00 00 00 00 00 .....\..........
0030 : 00 00 26 01 FF FF FF FF-FB D9 FF FF FF FF FF F9 ..&.............
0040 : 30 00 00 00 02 18 C1 FF-FF FF FF BB 25 00 00 00 0...........%...
0050 : 00 1C F5 20 00 00 00 AE-2F BF FF FF FF B3 21 80 ... ..../.....!.
0060 : 00 00 00 92 43 00 00 00-13 5C B7 FF FF FF EA 69 ....C....\.....i
0070 : A4 00 00 00 37 A9 E0 00-00 00 00 00 00 00 00 00 ....7...........
0080 : 00 13 CF A5                                     ....

MSG:4072 (u-Blox Proprietary)
CRC:13CFA5 13CFA5
MSG:4072 ID:   0 - Reference Station PVT
  1    0   1
EPOCH:155033999 (1 19:03:53.999)
 0  0
  104284.1782 -4749477.5187  4241927.4229
   -0.0016     0.0009     0.0046
    0.0000     0.0000
     0.00019459     -0.00008497     -0.00000218      0.00068704     -0.00035254      0.00059305
     0.00178366     -0.00039357      0.00037443      0.00634459     -0.00353687      0.00455996
    0.0000     0.0000
  0   0
1029 128:5
-----------------------------------------------------------------------------
SIZE:050   80
0000 : D3 00 4A FE 80 01 00 00-00 12 4F 69 5D C0 00 00 ..J.......Oi]...
0010 : 0A 00 00 00 00 00 00 00-01 02 A0 00 05 00 03 01 ................
0020 : 4B 00 06 02 00 FE C5 00-09 02 03 00 20 00 0F 03 K........... ...
0030 : 00 00 D1 00 39 03 03 02-08 00 3B 05 00 00 00 01 ....9.....;.....
0040 : 90 06 00 FE 6E 00 03 06-01 FC B3 00 07 49 EF 34 ....n........I.4

MSG:4072 (u-Blox Proprietary)
CRC:49EF34 49EF34
MSG:4072 ID:   1 - Reference Station Timing
  0    0   1
EPOCH:155034999 (1 19:03:54.999)
       0 COUNT:10
GPS
 L1   0   0  0.00  0.00
 L2   0   1  6.72  0.05
 L5   0   3  3.31  0.06
GALILEO
 L1   2   0 -3.15  0.09
 L5   2   3  0.32  0.15
BEIDOU
 L1   3   0  2.09  0.57
 L5   3   3  5.20  0.59
QZSS
 L1   5   0  0.00  4.00
GLONASS
 L1   6   0 -4.02  0.03
 L2   6   1 -8.45  0.07
-----------------------------------------------------------------------------
SIZE:133  307
0000 : D3 01 2D 43 50 00 24 F6-95 DC 00 00 06 58 08 44 ..-CP.$......X.D
0010 : 00 00 00 00 20 00 81 00-6F FE 9F 23 27 28 23 25 .... ...o..#'(#%
0020 : 21 A4 26 00 00 00 00 38-ED C2 6D D8 81 AD 1E 29 !.&....8..m....)
0030 : 64 03 8D F0 4F E0 1F 72-FB F1 FE AF E8 41 26 7F d...O..r.....A&.
0040 : 0A 78 00 37 9A 98 31 C2-BE 1B D6 4E 2E 41 64 A4 .x.7..1....N.Ad.
0050 : 16 3F BB 19 74 F9 A6 7D-1A 99 FF BD B7 7C CC 88 .?..t..}.....|..
0060 : A5 3A 67 8E 96 5A E8 E8-8B F8 54 75 86 8A 48 10 .:g..Z....Tu..H.
0070 : 74 37 FE 9B F0 64 3B E0-3D D4 88 5F 7F BF 89 2E t7...d;.=.._....
0080 : 3F 78 F8 97 8D F7 48 66-B3 20 3E BC 68 65 0F 3F ?x....Hf. >.he.?
0090 : F1 8E CF C2 2D 10 30 89-EF 9D 91 A7 96 93 9F A2 ....-.0.........
00A0 : 38 F8 11 75 D8 12 E7 24-95 25 4A 72 9C A6 D6 C5 8..u...$.%Jr....
00B0 : 08 6C A7 29 CA 71 3C A7-29 AB 80 02 9C FB 43 60 .l.).q<.).....C`
00C0 : 00 23 10 B0 27 0A 02 80-90 23 07 C2 F0 BC 2E 0B .#..'....#......
00D0 : 42 80 B8 21 06 02 00 9C-27 03 51 06 BF 33 B3 E7 B..!....'.Q..3..
00E0 : B4 CF D9 6A 12 DC E5 99-8D 4D 9A C1 75 56 62 F9 ...j.....M..uVb.
00F0 : C7 73 9A 5F CC 57 9D 7E-F9 28 ED D2 2A 80 00 00 .s._.W.~.(..*...
0100 : 00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00 ................
0110 : 00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00 ................
0120 : 00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00 ................
0130 : C3 C5 58                                        ..X

MSG:1077 (GPS-MSM data)
CRC:C3C558 C3C558
MSG:1077 ID:   0 EPOCH:155034999 (1 19:03:54.999) SYNC:0 SEQ:0
Satellite Mask: 0000110010110000000100001000100000000000000000000000000000000000 Sats: 8
Signal Mask:    01000000000000010000001000000000 Sigs: 3
Cell Mask:      110111111111110100111110 Cells: 19
    S#  0        S#  1        S#  2        S#  3        S#  4        S#  5        S#  6        S#  7
           5            6            9           11           12           20           25           29 Satellite Number
          70           78           80           70           74           67           72           76 Rough Range Integer ms
           0            0            0            0            0            0            0            0 Satellite Info
         455          440          310          945           13          419          788          712 Rough Range Modulo ms
         454         -503         -256         -283         -520          -43         -190          588 Rough Phase-Range Rate

21118680.623 23512628.796 24074154.122 21262135.998 22188447.851 20208763.670 21815756.641 22992676.251 Rough Range

    C#  0        C#  1        C#  2        C#  3        C#  4        C#  5        C#  6        C#  7        C#  8        C#  9        C# 10        C# 11        C# 12        C# 13        C# 14        C# 15        C# 16        C# 17        C# 18
           5            5            6            6            6            9            9            9           11           11           11           12           12           20           25           25           25           29           29 Satellite Number
           0            1            0            1            2            0            1            2            0            1            2            0            1            0            0            1            2            0            1 Signal Number
       -7857          111       217862       230780       228041      -238462      -224126      -229514       208543       216314       217919       -33938       -26223        84596      -200238      -215599      -192129        43243        53577 Fine Pseudo Range
      134790       -11394       821116       506513       782327      -973369     -1106158      -934167       841316       513933       827879      -118311      -506462       397629      -806348      -863629      -768225       143035       154852 Fine Phase-Range
         586          586          595          595          595          429          552          269          595          595          595          551          595          595          348            0          334          502          539 Lock-Time Indicator
           0            0            0            0            0            0            0            0            0            0            0            0            0            0            0            1            0            0            0 Half Cycle Ambiguity
      49.000       44.000       39.000       40.000       40.000       36.000       35.000       31.000       47.000       47.000       46.000       45.000       40.000       46.000       33.000       24.000       32.000       39.000       39.000 CNR
     0.67920      0.69080     -0.62970     -0.62200     -0.61640     -0.95960     -0.93160     -0.98320     -0.55240     -0.53710     -0.54600     -0.74310     -0.72390     -0.65050     -0.16540     -0.15770     -0.21030      0.91430      0.93010 Fine Phase-Range-Rate

    -4.38740      0.06198    121.65566    128.86916    127.33968   -133.15884   -125.15352   -128.16222    116.45186    120.79124    121.68749    -18.95122    -14.64311     47.23900   -111.81429   -120.39198   -107.28617     24.14719     29.91777 Fine Range
    18.81692     -1.59062    114.62922     70.71010    109.21421   -135.88401   -154.42158   -130.41134    117.44918     71.74594    115.57335    -16.51642    -70.70298     55.50970   -112.56759   -120.56411   -107.24555     19.96794     21.61761 Fine Phase-Range

21118676.235 21118680.685 23512750.451 23512757.665 23512756.135 24074020.964 24074028.969 24074025.960 21262252.450 21262256.790 21262257.686 22188428.900 22188433.208 20208810.909 21815644.827 21815636.249 21815649.355 22992700.399 22992706.169 range
110979514.61  86477460.28 123560300.66  96280573.92  92269034.44 126509819.71  98579004.39  94471640.09 111733896.01  87065186.37  83437642.26 116600993.65  90857695.25 106198061.57 114641983.38  89331382.87  85609294.18 120827434.15  94151254.15 phase
  -2389.4245   -1861.9155    2646.6685    2062.3020    1976.3585    1350.2278    1052.1284    1008.3127    1490.0689    1161.0646    1112.6990    2736.4817    2132.2748     229.2824     999.2919     781.7794     746.2645   -3094.7911   -2411.5678  phase delta (calc)
   2389.3553    1861.8828   -2646.5919   -2062.2479   -1976.2989   -1350.3318   -1052.0920   -1008.4573   -1490.0779   -1161.0370   -1112.6954   -2736.5235   -2132.2773    -229.3849    -999.3259    -778.6640    -746.4261    3094.7655    2411.5703 doppler
21118676.235 21118680.685 23512750.451 23512757.665 23512756.135 24074020.964 24074028.969 24074025.960 21262252.450 21262256.790 21262257.686 22188428.900 22188433.208 20208810.909 21815644.827 21815636.249 21815649.355 22992700.399 22992706.169 rtcm3torinex range
110979514.61  86477460.28 123560300.66  96280573.92  92269034.44 126509819.71  98579004.39  94471640.09 111733896.01  87065186.37  83437642.26 116600993.65  90857695.25 106198061.57 114641983.38  89331382.87  85609294.18 120827434.15  94151254.15 rtcm3torinex phase
  -2389.4245   -1861.9155    2646.6685    2062.3020    1976.3585    1350.2278    1052.1284    1008.3127    1490.0689    1161.0646    1112.6990    2736.4817    2132.2748     229.2824     999.2919     781.7794     746.2645   -3094.7911   -2411.5678  phase delta (calc)
  -2389.3553   -1861.8828    2646.5919    2062.2479    1976.2989    1350.3318    1052.0920    1008.4573    1490.0779    1161.0370    1112.6954    2736.5235    2132.2773     229.3849     999.3259     778.6640     746.4261   -3094.7655   -2411.5703 rtcm3torinex doppler

2025 80 [0FD.1]
-----------------------------------------------------------------------------
```

### MSM7, MSM4

u-Blox encodes a lot of unnecessary slack space as it over estimates the packet size during construction, this typically means they are 15-20% bigger than necessary (satelites * signals) != cells

### Support

sourcer32@gmail.com

