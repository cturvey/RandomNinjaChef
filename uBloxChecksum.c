//****************************************************************************
// Quick uBlox Fletcher Checksum Example - sourcer32@gmail.com
//****************************************************************************
//
//  The sum skips the sync bytes 0xB5,0x62, but includes the Class, ID, Length
//  and Payload data. Total data length is Payload + 8, 6 on the front, 2 on
//  the tail for the checksum
//
//  If this saves you several man-hours/days consider https://paypal.me/cliveone
//
//****************************************************************************

void ChecksumUBLOX(uint8_t *data) // Assumes buffer is large enough and modifyable
{
  int i, length;
  uint8_t a, b;

  a = 0; // Clear initial checksum bytes
  b = 0;

  length = data[4] + (data[5] << 8); // 16-bit Payload Length
  
  for(i=2; i<(length + 6); i++) // Sum over body
  {
    a += data[i];
    b += a;
  }

  data[i+0] = a; // Write checksum bytes into tail
  data[i+1] = b;
}

//****************************************************************************

// Fifo can be used to pass a structure for UART or hSerial type handle
//  to differentiate which device you're talking too, or simply enumerate

// switch((int)Fifo)
// {
//   case 0 : Serial.write(Buffer, Size); break;
//   case 1 :  Serial1.write(Buffer, Size); break;
//   case 2 :  Serial2.write(Buffer, Size); break;
//   default :
//      Serial1.write(Buffer, Size);
// }

void SendUBLOX(void *Fifo, int Size, uint8_t *Buffer)
{
  ChecksumUBLOX(Buffer); // Add/compute checksum bytes for packet 

  Serial1.write(Buffer, Size);  // Arduino style byte send
}

//****************************************************************************
