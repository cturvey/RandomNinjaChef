// https://portal.u-blox.com/s/question/0D52p0000BusjJ0CQI/time-information-in-ubxtimtm2

#include <windows.h>
#include <stdio.h>

typedef unsigned char uint8_t;
typedef unsigned long uint32_t;

uint8_t ubx_tim_tm[] = {
  0xB5,0x62,0x0D,0x03,0x1C,0x00,
  0x00,0xF5,0x4A,0x00,0x8F,0x08,0x8F,0x08,
  0x67,0x4F,0xD4,0x13,0x9F,0x05,0x0C,0x00,
  0x67,0x4F,0xD4,0x13,0x02,0xEF,0x0B,0x00,
  0x27,0x04,0x00,0x00,
  0xAA,0x58 };

void decode_tow(uint8_t *tim_tm)
{
  uint32_t towR, towF;
  uint32_t towSubMsR, towSubMsF;

  uint32_t hour, minute, second, milli;

// There are a lot of other flags to look at to determine time scale

  towR      = *((uint32_t *)&tim_tm[6 +  8]);
  towSubMsR = *((uint32_t *)&tim_tm[6 + 12]);

  towR %= 86400000; // ms/day

  hour = towR / 3600000;
  towR %= 3600000;

  minute = towR / 60000;
  towR %= 60000;

  second = towR / 1000;
  milli  = towR % 1000;

  printf("Rising  %02u:%02u:%02u.%03u%06u\n", hour, minute, second, milli, towSubMsR);

  towF      = *((uint32_t *)&tim_tm[6 + 16]);
  towSubMsF = *((uint32_t *)&tim_tm[6 + 20]);

  towF %= 86400000; // ms/day

  hour = towF / 3600000;
  towF %= 3600000;

  minute = towF / 60000;
  towF %= 60000;

  second = towF / 1000;
  milli  = towF % 1000;

  printf("Falling %02u:%02u:%02u.%03u%06u\n", hour, minute, second, milli, towSubMsF);
}

int main(int argc, char **argv[])
{
  decode_tow(ubx_tim_tm);

  return(1);
}
