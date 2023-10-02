//****************************************************************************
// Quick UBX Packet File Processing Demo - sourcer32@gmail.com
//****************************************************************************

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//****************************************************************************

// Or from stdint.h in Linux

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

//****************************************************************************

#define UBX_BUFFER_SIZE 4096

//****************************************************************************

uint8_t ubx_buffer[UBX_BUFFER_SIZE];

int ubx_index = 0;
int ubx_fill = 0;

//****************************************************************************

int process_packet(uint8_t *packet, uint16_t length)
{
  uint16_t i;
  uint8_t a, b;

  a = 0;
  b = 0;

  for(i=2; i<(length + 6); i++)
  {
    a += packet[i];
    b += a;
  }

  if ((a != packet[i+0]) || // Checksum Fail
      (b != packet[i+1]))
    return(1);

  // Print Whole Packets

  for(i=0; i<(length + 8); i++)
    fprintf(stdout, "%02X", packet[i]);

  fprintf(stdout, "\n");

  // Other user code to process and decode specific packets

  return(length + 8); // Data size total
}

//****************************************************************************

void process_block(int length, uint8_t *data)
{
  if ((ubx_fill + length) > UBX_BUFFER_SIZE)
  {
    fprintf(stderr, "UBX Buffer Overflowing");
    exit(1);
  }

  memcpy(&ubx_buffer[ubx_fill], data, length);
  ubx_fill += length;

  // Find Sync

  while(((ubx_fill - ubx_index) > 2) && (ubx_buffer[ubx_index+0] != 0xB5) && (ubx_buffer[ubx_index+1] != 0x62))
    ubx_index++;

  while((ubx_fill - ubx_index) >= 8) // Enough Data for UBX Packet Headers and Sums
  {
    length = (ubx_fill - ubx_index);

    if ((ubx_buffer[ubx_index + 0] == 0xB5) &&
        (ubx_buffer[ubx_index + 1] == 0x62))
    {
      uint16_t msg_length =
        ((uint16_t)ubx_buffer[ubx_index + 4] << 0) +
        ((uint16_t)ubx_buffer[ubx_index + 5] << 8);

      if ((msg_length + 8) > UBX_BUFFER_SIZE) // Too big of a packet
      {
        ubx_index++;
      }
      else if ((msg_length + 8) > length) // Not Enough data yet
      {
        break;
      }
      else
        ubx_index += process_packet(&ubx_buffer[ubx_index], msg_length);
    }
    else // No Sync Bytes, just consume
      ubx_index++;
  }

  length = (ubx_fill - ubx_index);

  if (length && ubx_index) // Move Down buffer
  {
    memcpy(ubx_buffer, &ubx_buffer[ubx_index], length);
    ubx_index = 0;
    ubx_fill = length;
  }
}

//****************************************************************************

int main(int argc, char **argv)
{
  FILE *f;

  if (argc > 1)
  {
    f = fopen(argv[1], "rb");

    if (f)
    {
      while(1)
      {
        uint8_t buffer[64];
        int bytes_read;

        bytes_read = fread(buffer, 1, sizeof(buffer), f);

        if (bytes_read > 0) process_block(bytes_read, buffer);
        else break; // Error or End-of-File
      }

      fclose(f);
    }
    else
      fprintf(stderr, "Can't open file '%s'\n", argv[1]);
  }

  return(1);
}

//****************************************************************************

