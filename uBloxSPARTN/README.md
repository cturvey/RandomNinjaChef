## SPARTN RESOURCES
If this saves you several man-hours/days consider donating / contributing
  *  https://paypal.me/cliveone  
  *  https://www.venmo.com/u/cliveone
  *  Now also accepting coffee  https://buymeacoffee.com/cliveone

Related to
  *  https://portal.u-blox.com/s/question/0D52p0000CimfsOCQQ/spartn-initialization-vector-iv-details

Thanks to Helge from the uBlox forum for doing some quick plots of the GAD I extracted from the live data, should give a better visualization of the coverage.

Some additional discussions here

  *  https://portal.u-blox.com/s/question/0D52p0000Cw5Qz5CQE/how-to-enable-the-ubxrxmpmp-message-from-d9s
  *  https://portal.u-blox.com/s/question/0D52p0000DbrPUBCQ2/which-inductor-value-should-i-useand-do-i-need-the-ferrite-bead-in-my-design
  *  https://portal.u-blox.com/s/question/0D52p0000DZMlwtCQD/neo-d9c-and-zed-f9p-sharing-same-active-antenna
  *  https://forum.sparkfun.com/viewtopic.php?f=116&t=58895
  *  https://portal.u-blox.com/s/question/0D5Oj000006sloaKAA/looking-for-packet-format-for-spartn-correction-packets

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

Primary difference is the L-Band frequency the respective Inmarsat is parked on US 1556.29 MHz, EU 1545.26 MHz

  *  https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxSPARTN/NEO-D9S_FW104_EU-00_gen9cfg.txt

  *  https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxSPARTN/NEO-D9S_FW104_US-00_gen9cfg.txt

  *  US Satellite: 97.6W INMARSAT 4-F3 - 1556.29 MHz
  *  EU Satellite: 24.8E ALPHASAT (INMARSAT-4A F4) - 1545.26 MHz

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

INPAQ L-BAND  1525~1559 MHz  used in u-Blox Kits (should be around $11, Mikro-E had them closer to $60)

![alt text](InpaqLBand.jpg?raw=true)

https://www.digikey.com/en/products/detail/inpaq-technology-co-ltd/LBAND01D-S6-00/20511929

https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxSPARTN/LBAND01D-S6-00.pdf

https://download.mikroe.com/documents/datasheets/LBAND01D-S6-00_datasheet.pdf

RTL-SDR LBAND (around $60 for kit, findable on Amazon)

![alt text](lband_patch_main2_500x500.jpg?raw=true)

https://www.amazon.com/RTL-SDR-Blog-1525-1637-Inmarsat-Iridium/dp/B07WGWZS1D

https://www.rtl-sdr.com/rtl-sdr-blog-active-l-band-patch-antenna-for-inmarsat-iridium-gps-back-in-stock/

## USB SPARTN-MOUSE
![alt text](usb_spartn_mouse.jpg?raw=true)

If in the USA, and interested in a $99 SPARTN-Mouse please drop me an email

