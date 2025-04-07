// From some conversations, ramblings and postings on the u-Blox forum
//****************************************************************************
//
// Unpacking UBX-NAV-RELPOSNED  sourcer32@gmail.com
//
//  If this saves you several man-hours/days consider https://paypal.me/cliveone
//
//****************************************************************************

#define DEG_TO_RAD              (PI / 180.0)
#define RAD_TO_DEG              (180.0 / PI)

// Should be byte packed structures

typedef struct _RELPOSNED_V0 {
  uint8_t   version;      // 0 40-byte (0x30) NEO-M8P
  uint8_t   reserved1;
  uint16_t  refStationId;
  uint32_t  iTOW;         // ms
  int32_t   relPosN;      // cm
  int32_t   relPosE;
  int32_t   relPosD;
  int8_t    relPosHPN;    // 0.1 mm
  int8_t    relPosHPE;
  int8_t    relPosHPD;
  uint8_t   reserved2;
  uint32_t  accN;         // mm
  uint32_t  accE;
  uint32_t  accD;
  uint32_t  flags;
} RELPOSNED_V0;

typedef struct _RELPOSNED_V1 {
  uint8_t   version;      // 1 64-byte (0x48) ZED-F9P
  uint8_t   reserved1;
  uint16_t  refStationId;
  uint32_t  iTOW;         // ms
  int32_t   relPosN;      // cm
  int32_t   relPosE;
  int32_t   relPosD;
  int32_t   relPosLength; // cm
  int32_t   relPosHeading; // deg 1e-5
  uint32_t  reserved2;
  int8_t    relPosHPN;    // 0.1 mm
  int8_t    relPosHPE;
  int8_t    relPosHPD;
  int8_t    relPosHPLength;
  uint32_t  accN;         // mm
  uint32_t  accE;
  uint32_t  accD;
  uint32_t  accLength;
  uint32_t  accHeading;   // deg 1e-5
  uint32_t  reserved3;
  uint32_t  flags;
} RELPOSNED_V1;

// Flags
// Bit 0    gnssFixOK
// Bit 1    diffSoln
// Bit 2    relPosValid
// Bit 3..4 carrSoln  0=No Carrier Phase, 1=Float, 2=Fixed
// Bit 5    isMoving (operating moving base mode)
// Bit 6    refPosMiss
// Bit 7    refObsMiss
// Bit 8    relPosHeadingValid F9P
// Bit 9    relPosNormalized  F9H

//****************************************************************************

void Dump013C_V1(BYTE *p)
{
  RELPOSNED_V1 relposned; // aligned local buffer
  unsigned int itow;
  double tow;
  double n, e, d;
  double base, ang;
  double roll;
  char type = '-';
  int valid = 0;

  memcpy(&relposned, p, sizeof(relposned));

  tow = ((double)relposned.iTOW * 1e-3);
  printf("%9u %10.3lf RELPOSNED %08X\n", relposned.iTOW, tow, relposned.flags);

  if (relposned.flags & 0x04) // relposvalid
  {
    n = ((double)relposned.relPosN * 1e-2) + ((double)relposned.relPosHPN * 1e-4); // metres
    e = ((double)relposned.relPosE * 1e-2) + ((double)relposned.relPosHPE * 1e-4);
    d = ((double)relposned.relPosD * 1e-2) + ((double)relposned.relPosHPD * 1e-4);

    ang = atan2(e, n) * RAD_TO_DEG; // +/- 180 degrees
    base = sqrt(n*n + e*e + d*d);

    roll = atan2(d, base) * RAD_TO_DEG;

    if (ang < 0.0) ang += 360.0; // 0..360 degrees

    if (base < 15.0) // Should be around 1.03 m
      valid = 1;

    switch((relposned.flags >> 3) & 3)
    {
      case 0 : type = 'A'; break;
      case 1 : type = 'B'; break; // FLOAT
      case 2 : type = 'C'; break; // FIXED
    }
  }

  {
    char string[128];

    if (valid)
      sprintf(string,"%6.3lf %6.2lf %c", base, ang, type);
    else
      sprintf(string,"XX.XXX XXX.XX -");

    puts(string);
  }
}

//****************************************************************************

void Dump013C(int Size, uint8_t *Buffer) // NAV-RELPOSNED
{
  uint8_t *p = Buffer + 6;
  uint32_t flags;
  uint32_t itow;
  double tow;
  double n, e, d;
  double acc;
  double base, ang;

  if (Size == 8)
  {
    printf("NAV-RELPOSNED - NULL\n"); // Request
    return;
  }

  if (p[0] == 0) // Version 0
  {
    printf("NAV-RELPOSNED %10u %d %3d %3d %4d\n",
      *((uint32_t *)&p[0x04]),   // iTOW
      *((uint8_t  *)&p[0x00]),   // version (0)
      *((uint8_t  *)&p[0x01]),   // reserved1
      *((uint8_t  *)&p[0x17]),   // reserved2
      *((uint16_t *)&p[0x02]));  // refstationId

    printf("%10d %10d %10d\n",
      *((int32_t *)&p[0x08]), // relPos NED
      *((int32_t *)&p[0x0C]),
      *((int32_t *)&p[0x10]));

    printf("%10d %10d %10d\n",
      *((int8_t *)&p[0x14]), // relPosHP NED
      *((int8_t *)&p[0x15]),
      *((int8_t *)&p[0x16]));

    printf("%10u %10u %10u\n",
      *((uint32_t *)&p[0x18]), // acc NED
      *((uint32_t *)&p[0x1C]),
      *((uint32_t *)&p[0x20]));

    printf("%08X\n",
      *((uint32_t *)&p[0x24]));

    itow = *((uint32_t *)&p[0x04]);
    tow = (double)itow * 1e-3;

    printf("TOW:%10.3lf  %s\n", tow, decodeitow(itow)); // to milliseconds

    flags = *((uint32_t *)&p[0x24]);

    if (flags & 1)
      puts("Valid Fix");

    if (flags & 2)
      puts("Differential Corrections Applied");

    if (flags & 4)
      puts("Relative Position Components Valid");

    switch((flags >> 3) & 0x03)
    {
      case 0 : puts("No Carrier Phase Range Solution"); break;
      case 1 : puts("Float Solution"); break; // No fixed integer carrier phase measurements
      case 2 : puts("Fixed Solution"); break; // One or more fixed integer carrier phase measurements
      default : puts("Unknown Carrier Phase Solution");
    }

    if (flags & 4)
    {
      n = (double)*((int32_t *)&p[0x08]) + ((double)*((int8_t *)&p[0x14]) * 1e-2); // cm
      e = (double)*((int32_t *)&p[0x0C]) + ((double)*((int8_t *)&p[0x15]) * 1e-2);
      d = (double)*((int32_t *)&p[0x10]) + ((double)*((int8_t *)&p[0x16]) * 1e-2);

      printf("%13.4lf %13.4lf %13.4lf\n", n, e, d); // NED cm

      base = sqrt(n*n + e*e + d*d);
      ang = atan2(e, n) * RAD_TO_DEG;
      if (ang < 0.0) ang += 360.0f;
      printf("%13.4lf %10.6lf (calc)\n", base, ang); // cm deg
    }
    else
      puts("NED Invalid");
  }
  else if (p[0] == 1) // Version 1
  {
    double l, h;

    printf("NAV-RELPOSNED %10u %d %3d %10d %10d %4d\n",
      *((uint32_t *)&p[0x04]),  // iTOW
      *((uint8_t  *)&p[0x00]),  // version (1)
      *((uint8_t  *)&p[0x01]),  // reserved0
      *((uint32_t *)&p[0x1C]),  // reserved1
      *((uint32_t *)&p[0x38]),  // reserved2
      *((uint16_t *)&p[0x02])); // refstationId

    printf("%10d %10d %10d %10d %10d\n",
      *((int32_t *)&p[0x08]),   // relPos NED
      *((int32_t *)&p[0x0C]),
      *((int32_t *)&p[0x10]),
      *((int32_t *)&p[0x14]),   // relPosLength
      *((int32_t *)&p[0x18]));  // relPosHeading

    printf("%10d %10d %10d %10d\n",
      *((int8_t *)&p[0x20]),    // relPosHP NED
      *((int8_t *)&p[0x21]),
      *((int8_t *)&p[0x22]),
      *((int8_t *)&p[0x23]));   // relPosLengthHP

    printf("%10u %10u %10u %10u %10u\n",
      *((uint32_t *)&p[0x24]),  // acc NED 0.1 mm  100 -> 1 cm
      *((uint32_t *)&p[0x28]),
      *((uint32_t *)&p[0x2C]),
      *((uint32_t *)&p[0x30]),  // accLength 0.1mm
      *((uint32_t *)&p[0x34])); // accHeading 1e-5 deg   51071 -> 0.51071 degrees

    flags = *((uint32_t *)&p[0x3C]); // 60

    if (flags & 1)
      puts("Valid Fix");

    if (flags & 2)
      puts("Differential Corrections Applied");

    if (flags & 4)
      puts("Relative Position Components Valid");

    if (flags & 32)
      puts("Is Moving");

    if (flags & 64)
      puts("Reference Position Missing");

    if (flags & 128)
      puts("Reference Observations Missing");

    if (flags & 256)
      puts("Relative Position Heading Valid");

    if (flags & 512)
      puts("Relative Position Normalized");

    switch((flags >> 3) & 0x03)
    {
      case 0 : puts("No Carrier Phase Range Solution"); break;
      case 1 : puts("Float Solution"); break; // No fixed integer carrier phase measurements
      case 2 : puts("Fixed Solution"); break; // One or more fixed integer carrier phase measurements
      default : puts("Unknown Carrier Phase Solution");
    }

    if (flags & 0x004)
    {
      n = (double)*((int32_t *)&p[0x08]) + ((double)*((int8_t *)&p[0x20]) * 1e-2); // cm
      e = (double)*((int32_t *)&p[0x0C]) + ((double)*((int8_t *)&p[0x21]) * 1e-2);
      d = (double)*((int32_t *)&p[0x10]) + ((double)*((int8_t *)&p[0x22]) * 1e-2);

      l = (double)*((int32_t *)&p[0x14]) + ((double)*((int8_t *)&p[0x23]) * 1e-2); // cm -> m

      acc = (double)*((uint32_t *)&p[0x30]) * 1e-4;  // mm to m   Acc Length

      printf("%13.4lf %13.4lf %13.4lf  NED cm (ACC: %5.2lf m)\n", n, e, d, acc); // NED cm
    }
    else
      puts("NED Invalid");

    if (flags & 0x100) // Heading Valid
    {
      h = ((double)*((int32_t *)&p[0x18]) * 1e-5);    // deg 1e-5  Heading

      acc = (double)*((uint32_t *)&p[0x34]) * 1e-5;   // deg 1e-5  Heading Accuracy
    }
    else
      puts("Heading Invalid");

    if ((flags & 0x104) == 0x104)
      printf("%13.4lf %10.6lf                   BASELINE cm degrees (ACC: %7.5lf deg)\n", l, h, acc); // cm deg
    else if (flags & 0x100)
      printf("              %10.6lf                   BASELINE cm degrees (ACC: %7.5lf deg)\n", h, acc); // cm deg
    else if (flags & 0x004)
      printf("%13.4lf                            BASELINE cm\n", l); // cm deg

    if (flags & 0x004) // Cross-check computation from baseline vector
    {
      base = sqrt(n*n + e*e + d*d);
      ang = atan2(e, n) * RAD_TO_DEG;
      if (ang < 0.0) ang += 360.0f;

      // NED accuracies
      n = (double)*((uint32_t *)&p[0x24]) * 1e-4; // mm to m
      e = (double)*((uint32_t *)&p[0x28]) * 1e-4; // mm to m
      d = (double)*((uint32_t *)&p[0x2C]) * 1e-4; // mm to m
      acc = sqrt(n*n + e*e + d*d);

      printf("%13.4lf %10.6lf (calc)  (ACC: %5.2lf m)\n", base, ang, acc); // cm deg
    }
  }

}

//****************************************************************************


