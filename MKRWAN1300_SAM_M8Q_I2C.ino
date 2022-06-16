/*
  uBlox SAM-M8Q I2C demo for MKR WAN 1300
   15-June-2022
   Copyright (C) 2022 C Turvey (aka sourcer32, clive1)
   All Rights Reserved
*/


/*
 MKR WAN 1300

 Serial  USB

 I2C  D11/PA08 SDA, D12/PA09 SCL,  SERCOM2

*/

#include <Wire.h>  // Arduino standard I2C/Two-Wire Library

//***************************************************************************

#define LORA_BAUD    115200 // LoRa Baud Rate
#define ARDUINO_BAUD 115200 // Arduino Rail Baud Rate

#define FAST_BLINK   100
#define FAST_ESCAPE 20000

#define NORMAL_BLINK 200

//***************************************************************************

void dumpdata(size_t size, uint8_t *buffer)
{
  char str[64];

  while(size)
  {
    char *s = str;
    size_t i = size;
    if (i > 16) i = 16;
    size -= i;
    while(i--)
      s += sprintf(s, "%02X ", *buffer++);
    Serial.println(str);
  }
}

//***************************************************************************
// Quick I2C
//***************************************************************************

#define UBX_I2C_ADDR 0x42

// Assumptions, you'll send at least 3-bytes, ideally a whole packet
//  min packet size with nothing would be 6 bytes

#define MAX_I2C_UBX_SEND 32 // This is some mythical limitation of Arduino Library

int ubx_send_i2c(size_t size, const uint8_t *buffer) // sourcer32@gmail.com
{
  while(size)
  {
    size_t length = size;

    // A method to get at least 5 bytes left to send in final packet
    if (length > MAX_I2C_UBX_SEND) length = MAX_I2C_UBX_SEND - 5;

    // Begin transmission to the I2C slave device
    Wire.beginTransmission(UBX_I2C_ADDR);

    // Queue data array for transmission to the I2C device
    if (Wire.write(buffer, length) != length) // Was it accepted?
    {
      Wire.endTransmission(true); // Send and Close the I2C interface.
      return(0); // Failed
    }

    // Transmit the bytes and a stop message to release the I2C bus.
    if (Wire.endTransmission(true))
      return(0); // Failed

    size -= length;
    buffer += length;
  }

  return(1); // Succeed
}

//***************************************************************************

int ubx_read_i2c(uint8_t reg, size_t size, uint8_t *buffer)
{
  // Begin transmission to the I2C slave device
#ifdef DEBUG_UBX_I2C
  Serial.println("RD");
#endif //  DEBUG_UBX_I2C
  Wire.beginTransmission(UBX_I2C_ADDR);
  Wire.write(&reg, sizeof(reg));
  if (Wire.endTransmission(false))
    return(0); // Failed
  Wire.requestFrom(UBX_I2C_ADDR, size); // Request bytes
  while(Wire.available() < size) {}; // Wait for delivery
  Wire.readBytes(buffer, size);
  return(1); // Succeed
}

//***************************************************************************

void ubx_read_flush(void)
{
  uint8_t buffer[64]; // Buffer of some reasonable size to pull down content
  if (ubx_read_i2c(0xFD, 2, buffer)) // Read a byte count from registers 0xFD/0xFE, 2-bytes
  {
    uint16_t count = ((uint16_t)buffer[0] << 8) + (uint16_t)buffer[1]; // MSB first
#ifdef DEBUG_UBX_I2C
    char str[16];
    sprintf(str, "LEN:%04X", count);
    Serial.println(str);
#endif // DEBUG_UBX_I2C
    while(count) // Consume available data
    {
      size_t length = count;
      if (length > sizeof(buffer)) length = sizeof(buffer);
      if (!ubx_read_i2c(0xFF, length, buffer)) break; // Read block from register 0xFF
      dumpdata(length, buffer); // Dump or process/consume
      count -= length;
    }
  }
}

//***************************************************************************

void test_pos_llh_req(void)
{
  uint32_t start = millis();

  uint8_t ubx_nav_posllh_req[] = { 0xB5,0x62,0x01,0x02,0x00,0x00,0x03,0x0A }; // Zero payload request form

  ubx_send_i2c(sizeof(ubx_nav_posllh_req), ubx_nav_posllh_req);

#ifdef DEBUG_UBX_I2C
  Serial.println("X1");
#endif

  while((millis() - start) < 5000) // Pump interface for 5 seconds to illustrate
  {
    ubx_read_flush();
    delay(100);
  }

#ifdef DEBUG_UBX_I2C
  Serial.println("X2");
#endif
}

//***************************************************************************

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);

  // LoRa side initialization for MKR WAN 1300

  pinMode(LORA_BOOT0, OUTPUT);
  pinMode(LORA_RESET, OUTPUT);

  digitalWrite(LORA_RESET, LOW); // Added

  // Need to check pull up/down state of BOOT0 pin

  //digitalWrite(LORA_BOOT0, HIGH); // System Loader
  digitalWrite(LORA_BOOT0, LOW); // Normal Operation

  delay(200);

  digitalWrite(LORA_RESET, HIGH);


  while(!Serial) // (USB) Wait for Host Side Connection
  {
    static uint32_t start = millis();
    if ((millis() - start) >= FAST_BLINK) // Fast HeatBeat - YELLOW PA20
    {
      static uint32_t led = 0;
      digitalWrite(LED_BUILTIN, (led & 1) ? HIGH : LOW);
      if (led++ > (FAST_ESCAPE / FAST_BLINK)) break;
      start += FAST_BLINK;
    }
  }

  Serial.println("MKRWAN1300 SAM-M8Q I2C");

  Wire.begin(); // Bring up the I2C
  test_pos_llh_req(); // Poke the beast

  Serial.println("Infinite Loop...");
}

//***************************************************************************

void loop(void)
{
  static uint32_t start = millis();
  int i;
  static int led = 0;

  if ((millis() - start) >= NORMAL_BLINK)
  {
		static uint32_t	led	=	0;
    if (((led % 1000) == 0) && Serial) Serial.println("USB ALIVE");
    digitalWrite(LED_BUILTIN, (led & 1) ? HIGH : LOW);
    led++;
    start += NORMAL_BLINK;
  }
}

//***************************************************************************
