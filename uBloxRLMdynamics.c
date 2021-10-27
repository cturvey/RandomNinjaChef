// From some conversations, ramblings and postings on the u-Blox forum
//****************************************************************************

// Setting ROBOT LAWN MOWER (DYNAMIC=11), 2 M/S HORIZONTAL DYNAMICS
//  uBlox Series 9 (ZED-F9P HPS 1.21)

// I'll migrate this to using the VALKEY approach at some point,
//  some additional notes here, see also the Wheel Tick SW method in
//  another file.

// https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxRLMwheelticks.c

// CFG-NAVSPG-DYNMODEL = 11

// The RLM dynamic model requires wheel ticks with polarity from both rear wheels.

// Automatic wheel tick polarity detection should not be used. This is achieved
/   by the configuration items 
//   CFG-SFODO-COMBINE_TICKS = 1
//   CFG-SFODO-DIS_AUTODIRPINPOL = 1 
// It is also strongly recommended to configure the coarse wheel tick scaling
//  factor by the configuration item
//   CFG-SFODO-FACTOR

uint8_t ubx_cfg_nav5[] = { // CFG-NAV5
0xB5,0x62,0x06,0x24,0x24,0x00,
0x01,0x00,0x0B,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x5A,0x5E }; 

SendUBLOX(hUARTGPS, sizeof(ubx_cfg_nav5), ubx_cfg_nav5); // Sums and sends...
