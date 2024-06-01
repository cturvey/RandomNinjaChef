// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D5Oj00000H0dIIKAZ/custom-message-through-zedf9p
// https://portal.u-blox.com/s/question/0D52p00008HKClkCAH/what-is-the-ubx-message-class-0x08-id-0x05-
//****************************************************************************
//
// Demonstration of u-Blox Tunnelling - Forwarding Method
//
//   char str[] = "Hello";
//   TunnelForward(hUARTGPS, UBX_PORT_NUMBER_UART1, strlen(str), (uint8_t *)str);
//
// If this saves you several man-hours/days consider https://paypal.me/cliveone
//
//  1-Jun-2024 sourcer32@gmail.com
//
//****************************************************************************

#define UBX_PORT_NUMBER_I2C    0
#define UBX_PORT_NUMBER_UART1  1
#define UBX_PORT_NUMBER_UART2  2
#define UBX_PORT_NUMBER_USB    3
#define UBX_PORT_NUMBER_SPI    4

#define MAX_FWD_LENGTH 200 // Convenient, needs to be less than 1000

//******************************************************************************

void TunnelForward(void *hSerial, int Port, int Size, uint8_t *Buffer)
{
  uint8_t Cmd[MAX_FWD_LENGTH + 12];
  uint8_t *p = Cmd;

  if ((Size == 0) || (Size > MAX_FWD_LENGTH)) return; // Too small or big?

  *p++ = 0xB5;
  *p++ = 0x62;

  *p++ = 0x08; // TUN-FWD
  *p++ = 0x05;

  *p++ = (uint8_t)(((Size + 4) >> 0) & 0xFF); // Payload Length
  *p++ = (uint8_t)(((Size + 4) >> 8) & 0xFF);

  *p++ = 0x02; // Version
  *p++ = 1 << Port; // Bit Mask (Port: 0=I2C,1=UART1,2=UART2,3=USB,4=SPI)
  *p++ = 0;
  *p++ = 0;

  memcpy(p, Buffer, Size);

  SendUBLOX(hSerial, Cmd, (Size + 12)); // https://github.com/cturvey/RandomNinjaChef/blob/main/uBloxChecksum.c
}

//******************************************************************************

