//****************************************************************************
//
// Simple NMEA Checksum Tool - Copyright (C) 2007-2025 C Turvey
//
// MSVC cl -Ox NMEASUM.c
//
//****************************************************************************
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
//****************************************************************************
//
// Win32 Console Application
//
// Compile with MSVC command line cl -Ox NMEASUM.c
//
// A NMEA command can be processed as a single argument, they do not contain spaces, and
// you do not need to provide the initial $ or trailing checksum, for example
//
// NMEASUM PRTHS,U1OP,ALL=0,GSA=1,RMC=1
//
//  $PRTHS,U1OP,ALL=0,GSA=1,RMC=1*73
//
// NMEASUM PNMRX111,COLD PNMRX111,HOT PNMRX600,INFO
//
//  $PNMRX111,COLD*40
//  $PNMRX111,HOT*17
//  $PNMRX600,INFO*4D
//
// NMEASUM PUBX,40,ZDA,0,1,0,0,0,0
//
//  $PUBX,40,ZDA,0,1,0,0,0,0*45
//
//****************************************************************************
static const char Hex[] = "0123456789ABCDEF";
//****************************************************************************

void NMEAMessage(char *s)
{
  int i;
  int j;
  int Len;
  unsigned char Checksum;
  char Buffer[300];

  Len = strlen(s);

  j = 0;

  Buffer[j++] = '$'; // Add Initial $

  Checksum = 0; // Zero checksum

  for(i=0; i<Len; i++) // Compute XOR checksum across body
  {
    Checksum ^= (unsigned char)s[i];
    Buffer[j++] = s[i];
  }

  Buffer[j++] = '*'; // Add * termination
  Buffer[j++] = Hex[(Checksum >> 4) & 0x0F]; // Add hexidecimal checksum
  Buffer[j++] = Hex[(Checksum >> 0) & 0x0F];
  Buffer[j++] = '\r'; // Add CR, LF
  Buffer[j++] = '\n';
  Buffer[j++] = 0;

  puts(Buffer);
}

//****************************************************************************

int main(int argc, char **argv)
{
  int i;

  for(i=1; i<argc; i++) // Process each argument as a NMEA sentence
    NMEAMessage(argv[i]);

  return(0);
}

//****************************************************************************
