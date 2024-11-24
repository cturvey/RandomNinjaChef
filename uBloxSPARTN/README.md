## SPARTN RESOURCES
If this saves you several man-hours/days consider donating / contributing
  *  https://paypal.me/cliveone  
  *  https://www.venmo.com/u/cliveone
  *  https://buymeacoffee.com/cliveone

Related to
  *  https://portal.u-blox.com/s/question/0D52p0000CimfsOCQQ/spartn-initialization-vector-iv-details

Thanks to Helge from the uBlox forum for doing some quick plots of the GAD I extracted from the live data, should give a better visualization of the coverage.

Some additional discussions here

  *  https://portal.u-blox.com/s/question/0D52p0000Cw5Qz5CQE/how-to-enable-the-ubxrxmpmp-message-from-d9s
  *  https://portal.u-blox.com/s/question/0D52p0000DbrPUBCQ2/which-inductor-value-should-i-useand-do-i-need-the-ferrite-bead-in-my-design
  *  https://portal.u-blox.com/s/question/0D52p0000DZMlwtCQD/neo-d9c-and-zed-f9p-sharing-same-active-antenna
  *  https://forum.sparkfun.com/viewtopic.php?f=116&t=58895
  *  https://portal.u-blox.com/s/question/0D5Oj000006sloaKAA/looking-for-packet-format-for-spartn-correction-packets
  *  https://support.thingstream.io/hc/en-gb/articles/15127213672732-D9S-Signal-Troubleshooting-with-PointPerfect-L-band

The boards

  *  https://content.u-blox.com/sites/default/files/C101-D9S-AppBoard_UserGuide_UBX-20031865.pdf
  *  https://content.u-blox.com/sites/default/files/documents/C102-F9R_UserGuide_UBX-20029244.pdf

The protocol

  *  https://www.spartnformat.org/wp-content/uploads/210928_SPARTN_v2.0.1.pdf

Mini-Circuits RF Power Splitters, GP2S+ and SCN-3-16+

  *  https://www.minicircuits.com/pdfs/GP2S+.pdf

Service

  *  ThingStream
     *  https://portal.thingstream.io/register
     *  https://developer.thingstream.io/guides/location-services/pointperfect-getting-started
  *  Nordian (Brazil)
     *  https://www.nordian.com/point-perfect
     *  https://www.nordian.com/form

The configuration parameters were under NDA, but have been published here, openly, so will provide my scripts for them

https://learn.sparkfun.com/tutorials/gnss-correction-data-receiver-neo-d9s-hookup-guide/all

Primary difference is the L-Band frequency the respective Inmarsat is parked on US 1556.29 MHz, EU 1545.26 MHz, BR 1545.83 MHz

  *  https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxSPARTN/NEO-D9S_FW104_EU-00_gen9cfg.txt

  *  https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxSPARTN/NEO-D9S_FW104_US-00_gen9cfg.txt

  *  US/NA Satellite: 97.6W INMARSAT 4-F3 - 1556.29 MHz
  *  EU Satellite: 24.8E ALPHASAT (INMARSAT-4A F4) - 1545.26 MHz
  *  BR/SA Satellite: 97.6W INMARSAT 4-F3 - 1545.83 MHz

## SATELLITE VISIBILITY
Determine Satellite Visibility from your location

  *  https://www.groundcontrol.com/us/knowledge/look-angle-calculator/

```
Address: 51.5072, 0.1276 - London, England
Latitude: 51.5072°
Longitude: 0.1276°

Satellite: 24.8E ALPHASAT
Elevation: 26.7°
Azimuth (true): 149.6°
Azimuth (magn.): 148.7°
```
```
Address: 54.9069, -1.3838 - Sunderland, England
Latitude: 54.9069°
Longitude: -1.3838°

Satellite: 24.8E ALPHASAT
Elevation: 23.1°
Azimuth (true): 149.0°
Azimuth (magn.): 149.0°
```
```
Address: 52.5200, 13.4050 - Berlin, Germany
Latitude: 52.5200°
Longitude: 13.4050°

Satellite: 24.8E ALPHASAT
Elevation: 29.0°
Azimuth (true): 165.8°
Azimuth (magn.): 160.8°
```
```
Address: 48.886300, 9.545600 - Stuttgart, Germany
Latitude: 48.8863°
Longitude: 9.5456°

Satellite: 24.8E ALPHASAT
Elevation: 32.0°
Azimuth (true): 160.1°
Azimuth (magn.): 156.3°
```
```
Address: 40.1039,-105.1708 - Niwot CO
Latitude: 40.1039°
Longitude: -105.1708°

Satellite: 97.6W INMARSAT 4-F3
Elevation: 43.0°
Azimuth (true): 168.3°
Azimuth (magn.): 160.5°
```
```
Address: 41.7606, -88.3201 - Aurora IL
Latitude: 41.7606°
Longitude: -88.3201°

Satellite: 97.6W INMARSAT 4-F3
Elevation: 40.8°
Azimuth (true): 193.8°
Azimuth (magn.): 197.4°
```

## DIRECTIONAL ANTENNA PERFORMANCE
47.55 dBHz (13.625 Eb/NO [dB]), no demodulation errors
![alt text](neo_d9s_004.jpg?raw=true)

## GAD MAPS
![alt text](us_gad.jpg?raw=true)
![alt text](eu_gad.jpg?raw=true)
![alt text](au_gad.jpg?raw=true)
![alt text](jp_gad.jpg?raw=true)

## ANTENNAS
![alt text](beitian_all_frequency.jpg?raw=true)

INPAQ L-BAND  1525~1559 MHz  used in u-Blox Kits (should be around $11, Mikro-E had them closer to $60) small, roughly 35x35mm

![alt text](InpaqLBand.jpg?raw=true)

https://www.digikey.com/en/products/detail/inpaq-technology-co-ltd/LBAND01D-S6-00/20511929

https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxSPARTN/LBAND01D-S6-00.pdf

https://download.mikroe.com/documents/datasheets/LBAND01D-S6-00_datasheet.pdf

RTL-SDR LBAND (around $60 for kit, findable on Amazon) large, roughly 170x170mm

![alt text](lband_patch_main2_500x500.jpg?raw=true)

https://www.amazon.com/RTL-SDR-Blog-1525-1637-Inmarsat-Iridium/dp/B07WGWZS1D

https://www.rtl-sdr.com/rtl-sdr-blog-active-l-band-patch-antenna-for-inmarsat-iridium-gps-back-in-stock/

## USB SPARTN-MOUSE
![alt text](usb_spartn_mouse.jpg?raw=true)

If in the USA, and interested in a $99 SPARTN-Mouse please drop me an email

## NUCLEO + C101-D9S SPARTN RECEIVER / DECODER
![alt text](nucleo_spartn.jpg?raw=true)
```
---SPARTN--------------------------------------------------------------------
MSG:  1.2 TIMETAG: 468512280 / 0x1BECEE18 SIZE:  153 / 0x099
GPSTIME:2339.224280 (32)
IV: 02 4C 90 DF 67 70 C0 6C 2D 00 00 00 00 00 00 01
PAYLOAD CRYPTED
00000000 : 3A 18 89 B3 22 04 9F 0F-46 62 70 DB 49 73 97 6E :..."...Fbp.Is.n
00000010 : EC 42 B8 A0 E4 DC 1E 27-06 57 7D 19 49 51 C4 83 .B.....'.W}.IQ..
00000020 : 16 10 CB 97 9B C7 FB 1A-E9 68 0E A9 14 7C D7 F6 .........h...|..
00000030 : BA DE 58 5F A5 93 ED 4B-89 C6 18 BE 9D 14 67 44 ..X_...K......gD
00000040 : C4 53 09 2B 5E 90 8B BE-7E D1 87 2C 5B AB 69 FA .S.+^...~..,[.i.
00000050 : 23 76 E2 8B D9 B9 E1 B6-A0 18 08 AF 32 64 06 97 #v..........2d..
00000060 : 6F A9 2E 3C BB 12 9C B2-7E B9 A6 B5 58 84 F0 74 o..<....~...X..t
00000070 : 7F 72 4B 03 24 40 FC 8A-D3 00 F5 BD 97 0A 8F E6 .rK.$@..........
00000080 : 47 09 04 0B 4D C1 52 17-C0 F0 0A 60 21 A4 2E 35 G...M.R....`!..5
00000090 : C3 D8 74 F1 A3 64 94 44-03                      ..t..d.D.
 CRC:692F07 - 692F07
PAYLOAD PLAIN-TEXT (13.1.0.2)
REBUILT PACKET
00000000 : 73 02 4C AD 28 DF 67 70-C0 6C 06 88 45 00 14 A8 s.L.(.gp.l..E...
00000010 : 2B CD F2 FB 20 12 81 02-24 26 52 97 BF E0 09 3C +... ...$&R....<
00000020 : C0 AF 2A C2 4A 59 12 1E-17 83 15 32 45 3E 1F 04 ..*.JY.....2E>..
00000030 : 72 68 36 7D 8A 09 25 D2-5E FA 4B EC C7 8D 71 F8 rh6}..%.^.K...q.
00000040 : C8 1B 4A 40 29 50 57 43-99 E2 40 21 02 04 48 3C ..J@)PWC..@!..H<
00000050 : D0 EF C9 3F 9A 59 FA 9E-A4 83 F8 F3 E9 BE 69 08 ...?.Y........i.
00000060 : 2A EA 0A 82 A1 FC 44 D3-2C FE 43 F3 C5 97 D1 FC *.....D.,.C.....
00000070 : 18 1B 4A 80 29 70 56 83-91 F8 40 21 02 04 48 4C ..J.)pV...@!..HL
00000080 : C6 6F B8 3F DC 9A 21 9E-3C 83 80 F4 6A BE A9 07 .o.?..!.<...j...
00000090 : 31 EA F5 84 3A 08 C5 D1-AE FF D3 E8 45 9A 5D FC 1...:.......E.].
000000A0 : B8 27 C0 A4 B0 CC                               .'....
-----------------------------------------------------------------------------
 S:0 I:0  CRC PASS:47641 FAIL:0 TOTAL:47641 DEAD:2748
-----------------------------------------------------------------------------
 48457000 LOCK SYNC  44445000 1556290880 47.64 dBHz ##################----
-----------------------------------------------------------------------------
 48458000 LOCK SYNC  44446000 1556290879 47.88 dBHz ##################----
-----------------------------------------------------------------------------
 S:0 I:0  CRC PASS:47641 FAIL:0 TOTAL:47641 DEAD:2749
-----------------------------------------------------------------------------
 48459000 LOCK SYNC  44447000 1556290880 47.01 dBHz #################-----
```
![alt text](nucleo_spartn_u5.jpg?raw=true)
