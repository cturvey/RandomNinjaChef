//****************************************************************************
//  sourcer32@gmail.com 
//****************************************************************************

uint32_t getbitu(uint8_t *buf, int index, int count)
{
  uint32_t wrd = 0;

  while(count--)
  {
    wrd = (wrd << 1) | ((buf[index >> 3] & (0x80 >> (index & 7))) ? 1 : 0);
    index++;
  }

  return(wrd);
}

//****************************************************************************

void dump64(uint32_t Size, uint8_t *Buffer)
{
  const static char b64[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };
  int index = 0;
  int length = Size * 8; // bit length
  int pad = (Size % 3); // 3-bytes (24-bits) to 4-characters

  while(length >= 6)
  {
    int i;
    i = getbitu(Buffer, index, 6); index += 6;
    length -= 6;
    putchar(b64[i]);
  }

  if (length) // bits
  {
    int i;
    i = getbitu(Buffer, index, length); index += length;
    i <<= 6 - length;
    putchar(b64[i]);

    if (pad) pad = 3 - pad; // implicitly non-zero at this point

    while(pad--)
      putchar('=');
  }

  putchar('\n');
}

//****************************************************************************
