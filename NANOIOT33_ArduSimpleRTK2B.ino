/*

  Simple Uart Forwarding Exercise

  Arduino Nano IoT 33 board, proxy for RP2080, to ArduSimple RTK2B Shield

  5V (VBUS LINK MADE) to ARDUSIMPLE.5V_IN
  GND to ARDUSIMPLE.GND
  3V3 to ARDUSIMPLE.IOREF
  RX0 to ARDUSIMPLE.TX1

*/

void loop()
{
  int len = Serial1.available();
  while(len > 0) // Do blocks of bytes for efficiency
  {
    uint8_t buffer[128];
    int xferlen = len;
    if (len > sizeof(buffer)) xferlen = sizeof(buffer);
    Serial1.readBytes(buffer, xferlen);
    Serial.write(buffer, xferlen);
    len -=xferlen;
  }
}

void setup()
{
	Serial1.begin(38400); // ZED-F9P UART1 on this port
	while(!Serial1);

  Serial.begin(115200); // USB to PC
  while(!Serial);

	Serial.println();
	Serial.println("NANOIOT33_ArduSimpleRTK2B Starting");
}
