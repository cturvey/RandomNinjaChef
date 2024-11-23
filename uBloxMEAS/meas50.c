//****************************************************************************
//  sourcer32@gmail.com  23-Nov-2024 
//****************************************************************************

void Dump0286(uint32_t Size, uint8_t *Buffer) // RXM-MEAS50
{
  uint8_t *p = Buffer + 6;
  uint16_t Payload;
  int index = 0;
  int i;
  const char *gnss[] = { "GPS","GAL","BDS","GLO" };

  Payload = *((uint16_t *)&Buffer[4]);

  if ((Size == 8) || (Payload == 0)) // Request Packet
    return;

  DumpData(Payload, Buffer + 6);
  putchar('\n');

  dump64(Size, Buffer);
  putchar('\n');

  for(i=0; i<13; i++)
  {
    int gnssId;
    int svId;
    int chips;
    int dop;

    gnssId = getbitu(p, index,  2); index += 2;
    svId   = getbitu(p, index,  6); index += 6;
    chips  = getbitu(p, index, 17); index += 17;

    if ((gnssId == 0) && (svId == 0) && (chips == 0)) break;

    printf("#%2d %d:%s %2d %6d %8.3lf ", i, gnssId, gnss[gnssId], svId+1, chips, (double)chips/128.0);

    if ((i >= 1) && (i <= 6))
    {
      dop    = getbitu(p, index, 12); index += 12;
      printf("%4d %4d", dop, dop*2);
    }

    putchar('\n');
  }

  printf("index:%d %d\n", index, getbitu(p, index, 3) );
}

//****************************************************************************
