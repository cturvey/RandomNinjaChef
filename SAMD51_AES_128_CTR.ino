// SAMD51 Hardware AES-128 CTR mode demo
//  For AdaFruit Grand Central CM4 or SparkFun Thing Plus - SAMD51

// If this saves you several man-hours/days consider https://paypal.me/cliveone
//  15-Oct-2024  sourcer32@gmail.com

//****************************************************************************

void aes_init(void)
{
  MCLK->APBCMASK.reg |= MCLK_APBCMASK_AES;
}

//****************************************************************************

void aes_ctr_encdec(const uint8_t *key, const uint8_t *in, uint8_t *out, size_t size, uint8_t iv[16])
{
  uint32_t *p;
  int word = 0;
   
  REG_AES_CTRLA = AES_CTRLA_KEYSIZE_128BIT;

  p = (uint32_t *)key;
  REG_AES_KEYWORD0 = p[0];  // processed via AES_CTRLA_ENABLE
  REG_AES_KEYWORD1 = p[1];
  REG_AES_KEYWORD2 = p[2];
  REG_AES_KEYWORD3 = p[3];

  // COUNTER == CTR
  //  Both ways ENCRYPTING to generate XOR pattern, and apply BYTE wise
  REG_AES_CTRLA |= AES_CTRLA_AESMODE_COUNTER | AES_CTRLA_CIPHER_ENC | AES_CTRLA_ENABLE; // Mode and initialize

  p = (uint32_t *)iv;
  REG_AES_INTVECTV0 = p[0]; // processed via AES_CTRLB_NEWMSG
  REG_AES_INTVECTV1 = p[1];
  REG_AES_INTVECTV2 = p[2];
  REG_AES_INTVECTV3 = p[3]; // 16-bit count (big-endian) is automatically incremented in CTR mode
  
  REG_AES_CTRLB |= AES_CTRLB_NEWMSG; // Initiate

  uint32_t *wi = (uint32_t *) in;   // need to do by word ?
  uint32_t *wo = (uint32_t *) out;
  
  while(size > 0)
  {
    size_t len = min(16, size);
    int i;

    for (i=0; i<4; i++) // CTR XOR's so padded junk doesn't matter, perhaps slightly beyond bounds
      REG_AES_INDATA = wi[i + word];

    REG_AES_CTRLB |=  AES_CTRLB_START; // Engage AES Engine
    while((REG_AES_INTFLAG & AES_INTENCLR_ENCCMP) == 0);  // Wait until complete

    if (len == 16) // Optimized for word lines, and then bytes
    {
      for (i=0; i<4; i++)
        wo[i + word] = REG_AES_INDATA;
    }
    else // handle trailing bytes, without exceeding bounds
    {
      uint32_t buffer[4];
      uint8_t *b = (uint8_t *)buffer;
      
      out += 4 * word;
      
      for (i=0; i<4; i++)
        buffer[i] = REG_AES_INDATA;
  
      for(i=0; i<len; i++)
        *out++ = *b++;
    }
      
    size -= len;
    word += 4;
  }
}

//****************************************************************************
