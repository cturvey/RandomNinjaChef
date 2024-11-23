//****************************************************************************
//  sourcer32@gmail.com  23-Nov-2024 
//****************************************************************************

// bit stuff in base64.c

void Dump0280(uint32_t Size, uint8_t *Buffer) // RXM-MEASD12
{
  uint8_t *p = Buffer + 6;
  uint16_t Payload;
  int index = 0;
  int i, j;

  Payload = *((uint16_t *)&Buffer[4]);

  if ((Size == 8) || (Payload == 0)) // Request Packet
    return;

  DumpData(Payload, Buffer + 6); // Payload Dump Hex/ASCII
  putchar('\n');

  dump64(Size, Buffer); // UBX Packet in BASE64
  putchar('\n');

  for(i=0; i<96; i++) // Payload in Binary
    putchar('0' + getbitu(p, i, 1));
  putchar('\n');

  putchar('\n');

  i = getbitu(p, index,  6); index += 6; // Unknown, records present or not?
  printf("%2d\n", i);

  putchar('\n');

  for(i=0; i<6; i++) // 5 or 17-bits
  {
    int a, b;

    a = getbitu(p, index,  5); index += 5;  // svId-1  GPS only [1..32]

    if (i) // Record#0 is the referential Doppler Basis
    {
      b = getbitu(p, index, 12); index += 12; // doppler / 2.5

      printf("#%d  %2d %4d %6.1lf\n", i, a+1, b, (double)b*2.5);
    }
    else
      printf("#%d  %2d\n", i, a+1);
  }
  printf("index:%d\n", index);
  putchar('\n');
}

//****************************************************************************
