// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p00009IthhBCAR/ublox-rtcm-wrapper-specification
// https://portal.u-blox.com/s/question/0D52p00008HKD3FCAX/rtcm-msm4-vs-msm7
//****************************************************************************

uint32_t Crc24Quick(uint32_t Crc, uint32_t Size, unsigned char *Buffer) // sourcer32@gmail.com
{
  static const uint32_t crctab[] = { // Nibble lookup for Qualcomm CRC-24Q
    0x00000000,0x01864CFB,0x038AD50D,0x020C99F6,0x0793E6E1,0x0615AA1A,0x041933EC,0x059F7F17,
    0x0FA18139,0x0E27CDC2,0x0C2B5434,0x0DAD18CF,0x083267D8,0x09B42B23,0x0BB8B2D5,0x0A3EFE2E };

  while(Size--)
  {
    Crc ^= (uint32_t)*Buffer++ << 16; // Apply byte
    // Process 8-bits, 4 at a time, or 2 rounds
    Crc = (Crc << 4) ^ crctab[(Crc >> 20) & 0x0F];
    Crc = (Crc << 4) ^ crctab[(Crc >> 20) & 0x0F];
  }

  return(Crc & 0xFFFFFF); // Mask to 24-bit, as above optimized for 32-bit
}

//****************************************************************************

uint32_t DumpRTCM3(uint32_t Size, uint8_t *Buffer)
{
  if ((Size >= 6) && (Buffer[0] == 0xD3) && ((Buffer[1] & 0xFC) == 0x00))
  {
    uint32_t RecordSize = (((uint32_t)Buffer[1] & 3) << 8) + ((uint32_t)Buffer[2] << 0) + 6; // Header + CRC bytes
    uint16_t Msg = ((uint16_t)Buffer[3] << 4) + ((uint16_t)Buffer[4] >> 4); // 12-bit (0..4095)

    printf("RTCM3 %4d %4d %4d\n", RecordSize, Msg, Size);

    if (Size >= RecordSize)
    {
      // The CRC across the whole packet should sum to zero (remainder)
      if (Crc24Quick(0x000000, RecordSize, Buffer) == 0)
      {
        // Decompose message further here, or dispatch validated packet
        DumpData(RecordSize, Buffer);
      }
    }

    return(RecordSize);
  }

  return(Size);
}

//****************************************************************************
