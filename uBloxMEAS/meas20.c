//****************************************************************************
//  sourcer32@gmail.com  23-Nov-2024 
//****************************************************************************

// bit stuff in base64.c

void Dump0284(uint32_t Size, uint8_t *Buffer) // RXM-MEAS20
{
  uint8_t *p = Buffer + 6;
  uint16_t Payload;
  int index = 0;
  int i;
  const char *gnss[] = { "GPS","GAL","BDS","GLO" };

  int gnssId;
  int svId;
  int chips;
  int dop;

  Payload = *((uint16_t *)&Buffer[4]);

  if ((Size == 8) || (Payload == 0)) // Request Packet
    return;

  DumpData(Payload, Buffer + 6); // Payload Dump Hex/ASCII
  putchar('\n');

  dump64(Size, Buffer); // UBX Packet in BASE64
  putchar('\n');

  for(i=0; i<160; i++) // Payload in Binary
    putchar('0' + getbitu(p, i, 1));
  putchar('\n');

  putchar('\n');

  gnssId = getbitu(p, index,  2); index += 2;

  printf("%d:%s\n", gnssId, gnss[gnssId]);

  for(i=0; i<6; i++)
  {
    svId   = getbitu(p, index,  5); index += 5;
    chips  = getbitu(p, index, 16); index += 16; // 10.6 (Divide 64)

    printf("#%2d %2d ", i, svId+1);

    printf("%5d %8.3lf ", chips, (double)chips/64.0);

    if ((i >= 1) && (i <= 4))
    {
      dop    = getbitu(p, index, 8); index += 8; // Multiply 40
      printf("%4d %5d", dop, dop*40);
    }

    putchar('\n');
  }

  printf("index:%d\n", index);
  putchar('\n');
}

//****************************************************************************
