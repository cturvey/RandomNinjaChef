// From some conversations, ramblings and postings on the u-Blox forum
// https://portal.u-blox.com/s/question/0D52p0000CE0M8mCQF/sending-large-400-bytes-rtcm3-messages-to-zedf9p-via-i2c-not-working-works-however-for-small-25-bytes-messages
// https://portal.u-blox.com/s/question/0D52p00009ry69zCAA/zedf9pgpsrtk2-sample-code-for-rtcm3-correction-data-over-i2c-no-luck-with-serial
// https://portal.u-blox.com/s/question/0D52p00008Y5pLECAZ/rtcm-to-f9p-via-i2c-bus
// https://portal.u-blox.com/s/question/0D52p0000AXRCaWCQX/rtcm-via-i2c-on-zedf9p
//****************************************************************************
//
// If you lift this, remember where you got it, and how much time you wasted previously
//  with code and ideas that failed.
// 
// If this saves you several man-hours/days consider https://paypal.me/cliveone
//
// 30-Mar-2022  sourcer32@gmail.com
//
//****************************************************************************

#define UBX_I2C_ADDR 0x42

// Assumptions, you'll send at least 3-bytes, ideally a whole packet
//  min packet size with nothing would be 6 bytes

#define MAX_I2C_UBX_SEND 32

int ubx_send_rtcm3_i2c(size_t size, const uint8_t *buffer) // sourcer32@gmail.com
{
  while(size)
  {
    size_t length = size;

    // A method to get at least 5 bytes left to send in final packet
    if (length > MAX_I2C_UBX_SEND) length = MAX_I2C_UBX_SEND - 5;

    // Begin transmission to the I2C slave device
    Wire.beginTransmission(UBX_I2C_ADDR);

    // Queue data array for transmission to the I2C device
    if (Wire.write(buffer, length) != length) // Was it accepted?
    {
      Wire.endTransmission(true); // Send and Close the I2C interface.
      return(0); // Failed
    }

    // Transmit the bytes and a stop message to release the I2C bus.
    if (Wire.endTransmission(true))
      return(0); // Failed

    size -= length;
    buffer += length;
  }

  return(1); // Succeed
}

//****************************************************************************
// Going to add this as perhaps a secondary approach, where is pushes in
//  blocks which don't exceed the wire side buffering method.
// It opens once, and then feeds blocks without STOP phase, until the last ONE
// A NACK failure from the slave side will abort the transfer.

int ubx_send_rtcm3_i2c_ex(int size, const uint8_t *buffer)
{
  if (!size) return(1); // No data

  // Begin transmission to the I2C slave device
  Wire.beginTransmission(UBX_I2C_ADDR);

  if (size < 2) // I think this is the threshold
    Wire.write((uint8_t)0xFF); // Set 0xFF register for byte writes

  while(size)
  {
    int length = size;

    // Should be able to use wire's whole buffer, not Team Arduino here..
    if (length > MAX_I2C_UBX_SEND) length = MAX_I2C_UBX_SEND;

    // Queue data array for transmission to the I2C device
    if (Wire.write(buffer, length) != length) // Was it accepted? Oops!
    {
      Wire.endTransmission(true); // Send and Close the I2C interface.
      return(0); // Failed
    }

    // Transmit the bytes and a stop message to release the I2C bus
    //  if the last burst, otherwise don't stop, and push another

    if (Wire.endTransmission(size == length)) // true for last block, otherwise false
      return(0); // Failed

    size -= length;
    buffer += length;
  }

  return(1); // Succeed
}

//****************************************************************************
