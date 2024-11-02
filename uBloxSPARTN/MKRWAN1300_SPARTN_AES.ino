/*
  MKR WAN 1300 SPARTN AES
*/


/*
 MKR WAN 1300

 Serial  USB
 Serial1 Pin13/Pin14
 Serial2 Pin29/Pin26 LoRa Modem/Radio

 LoRa PA2/PA3 LPUART1 AF6
*/

#include <stdio.h>

#define WAN_BAUD 19200 // Original

//***************************************************************************

uint32_t lastMillis = 0;
int lastLED = 0;

//***************************************************************************

// Real World AES-128-CTR
//  Recovered SPARTN data from NEO-D9S via UBX-RXM-PMP using DUMPSPAR

uint8_t key[] = { 0x82,0x8F,0xEE,0x17,0x92,0xC0,0x5E,0xAA,0xFE,0x93,0xA4,0x38,0x68,0x3D,0x17,0x03 };

uint8_t iv[] = { 0x00,0x6B,0x10,0xC0,0xC3,0x98,0x10,0x6C,0x2D,0xC0,0x00,0x00,0x00,0x00,0x00,0x01 };

static const uint8_t in[] = {
  0xDB,0xB5,0x81,0xF8,0x7D,0xA3,0xCD,0x1E,0x76,0xC6,0xB8,0x3D,0x34,0x37,0xA9,0xAD,
  0xC4,0x42,0x0F,0x68,0xA2,0x21,0x94,0x21,0xA2,0x14,0x3C,0x5D,0x7D,0xB1,0xE7,0x1D,
  0xB6,0x43,0xCA,0x86,0xC6,0xAE,0x22,0xDD,0x12,0x36,0x45,0x70,0x5A,0x90,0xEC,0x3E,
  0x9B,0x29,0x07,0x39,0xDB,0xBC,0x9D,0x9B,0xD9,0x15,0x7E,0xAD,0x90,0xA7,0x7E,0x78,
  0xF0,0xC4,0x91,0xCB,0x3C,0x1E,0xEA,0x55,0xBC,0xF3,0x01,0x8E,0x2B,0x73,0x8F,0x69,
  0xDB,0xAD,0xBB,0x33,0x0D,0xCE,0x0B,0x46,0x60,0x30,0x58,0x0D,0xBA,0x85,0xF2,0x25,
  0x76,0x4E,0x96,0x6B,0x18,0x49,0xCA,0x97,0x10,0x3E,0xDB,0x42,0x15,0xA8,0x54,0x80,
  0xD6,0xB1,0x2E,0xFD,0xF4,0xD1,0xD1,0x07,0x30,0xFC,0x99,0x7E,0x70,0xB7,0xD9,0x23,
  0xA9,0x37,0x4A,0xB7,0x09,0x2D,0x12,0xF6,0x35,0xF3,0xCB,0x73,0xAE,0x68,0x33,0xB1,
  0xC2,0xF2,0xD4,0x84,0xCB,0x29,0x97,0x0B,0xDC,0x5C,0xFC,0xDA,0x14,0xDF,0xFA,0x82,
  0xCD,0x6E,0x93,0xCE,0xA5,0xED,0x10,0x5F,0x39,0xE5,0xF6,0x01,0xC6,0x84,0xD9,0x12,
  0x4B,0x9E,0xE4,0xB3,0xE2,0x02,0x1F,0x85,0x56,0x8C,0x9B,0xB2,0x1A,0x11,0x9D,0x02,
  0xA7,0x7E,0x4B,0xF3,0xB9,0x71,0xDB,0x87,0xFB,0x2F,0xD1,0x5E,0x2D,0x3B,0xE8,0x2F,
  0x1C,0xA6,0x07,0xE0,0x14,0x12 };

static const uint8_t out[] = {
  0x80,0xC1,0x00,0x9C,0x0A,0x09,0x21,0xF8,0xFA,0xFE,0xC3,0xFB,0xEF,0xF8,0x68,0xFC,
  0x13,0x83,0xDF,0xFF,0xF8,0x09,0x7D,0xFD,0xD7,0x03,0xEA,0x85,0xD0,0xB9,0xF8,0xFA,
  0xFD,0xD3,0xFD,0x30,0x23,0x77,0xC9,0x8B,0x83,0xDF,0xFF,0xF7,0xFA,0xFD,0xF9,0xD7,
  0x04,0x12,0x82,0x30,0x7D,0xF8,0xFA,0xFF,0xE4,0x09,0x90,0x34,0xF5,0x83,0x0B,0x83,
  0xDF,0xFF,0xF7,0xF5,0xFD,0xFD,0xA7,0x03,0xFE,0x6B,0xCD,0x99,0xF8,0xFB,0x00,0x6B,
  0xF1,0xF0,0x31,0x75,0xC0,0x0B,0x83,0xDF,0xFF,0xF7,0xCB,0xFD,0xF5,0x57,0x04,0x54,
  0x7A,0x6F,0xA5,0xF8,0xF8,0xFE,0x3C,0x26,0x4F,0xD5,0xF7,0xD8,0x0B,0x83,0xDF,0xFF,
  0xF7,0xE0,0x7D,0xF9,0x87,0x03,0xD4,0x74,0xCE,0xC5,0xF8,0xFA,0xFE,0xC3,0xF9,0x6F,
  0xF1,0x78,0x88,0x4B,0x83,0xDF,0xFF,0xF8,0x0F,0xBD,0xFE,0xE7,0x03,0xEE,0x82,0xF0,
  0x4D,0xF8,0xFA,0xFB,0x9B,0xFD,0x2F,0x9B,0x68,0x84,0x4B,0x83,0xDF,0xFF,0xF8,0x25,
  0xBE,0x03,0xF7,0x03,0xDA,0x7F,0xEF,0xFD,0xF8,0xFA,0xFD,0xC4,0x0B,0x0F,0xD8,0xF8,
  0x6C,0x13,0x83,0xDF,0xFF,0xF8,0x14,0xFE,0x07,0x87,0x03,0xDE,0x7F,0xCF,0xF1,0xF8,
  0xFA,0xFB,0x9B,0xF2,0x4F,0xA6,0x76,0xC0,0xD3,0x83,0xDF,0xFF,0xF7,0xD9,0x7D,0xF9,
  0xB7,0x04,0x42,0x84,0x50,0x90 };

//***************************************************************************

uint8_t junk[256];

//***************************************************************************

#include <Crypto.h>
#include <AES.h>
#include <CTR.h>

CTR<AES128> ctr;

//***************************************************************************

void aes_testing(void)
{
  ctr.setCounterSize(4);

  ctr.setKey(key, 16);

  ctr.setIV(iv, 16);
  ctr.encrypt(junk, in, sizeof(in));

  Serial.print("CTR memcmp a "); Serial.println(memcmp(out, junk, sizeof(out)) == 0 ? "PASS" : "FAIL");

  ctr.setIV(iv, 16);
  ctr.encrypt(junk, junk, sizeof(in)); // Re-Encrypt

  Serial.print("CTR memcmp b "); Serial.println(memcmp(in, junk, sizeof(in)) == 0 ? "PASS" : "FAIL");
}

//***************************************************************************

void setup()
{
  Serial1.begin(115200, SERIAL_8N1); // 115200 SERIAL_8E1 for System Loader
  Serial2.begin(WAN_BAUD, SERIAL_8N1); // 19200 8N1 LoRa Slave_AT

  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LORA_BOOT0, OUTPUT);
  pinMode(LORA_RESET, OUTPUT);

  // Need to check pull up/down state of BOOT0 pin

  //digitalWrite(LORA_BOOT0, HIGH); // System Loader
  digitalWrite(LORA_BOOT0, LOW); // Normal Operation

  digitalWrite(LORA_RESET, HIGH);

  while(!Serial); // (USB)

#if 0
  delay(200);
  digitalWrite(LORA_RESET, LOW);
  delay(200);
  digitalWrite(LORA_RESET, HIGH);
#endif

  lastMillis = millis();
  lastLED = 0;
  digitalWrite(LED_BUILTIN, lastLED ? HIGH : LOW);

  Serial.println("MKRWAN1300 SPARTN AES");

  aes_testing();
}

//***************************************************************************

void loop()
{
  if ((millis() - lastMillis) > 200)
  {
    lastMillis += 200;
    lastLED ^= 1;
    digitalWrite(LED_BUILTIN, lastLED ? HIGH : LOW);
  }
}

//***************************************************************************

