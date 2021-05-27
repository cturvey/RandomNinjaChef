# Quick-n-Dirty GNSS RTK Python Demo for u-Blox ZED-F9P / NEO-M8P
# I'm not a Python coder, this a quick ninja chef of something else I was working on earlier..
#  sourcer32@gmail.com

import serial
import sys
import math

GPS_SERIAL = "/dev/ttyACM0" # TTY for your receiver
GPS_BAUD = 38400

running = True


def truncate(number, decimals=0): # trying to remediate some fugly Python formatting issues
    # Returns a value truncated to a specific number of decimal places.
    if not isinstance(decimals, int):
        raise TypeError("decimal places must be an integer.")
    elif decimals < 0:
        raise ValueError("decimal places has to be 0 or more.")
    elif decimals == 0:
        return math.trunc(number)

    factor = 10.0 ** decimals
    return math.trunc(number * factor) / factor


def decimalDegrees(coordinates, digits, hemi): # crack NMEA (D)DDMM.mmmmm N/S E/W format
    parts = coordinates.split(".")

    if (len(parts) != 2):
        return coordinates

    if (digits > 3 or digits < 2):
        return coordinates

    left = parts[0]
    right = parts[1]

    degrees = float(left[:digits]) + ((float(left[digits:digits+2])) / 60.0) + (float("." + str(right)) / 60.0)

    if (hemi == 'S' or hemi == 'W'):
        degrees = degrees * -1.0

    return truncate(degrees, 8) # max 8 dp, if a minute is 1852m, a degree is 111120m, roughly, this is about a cm


def getPositionData(gps):

    data = gps.readline().decode('ascii', errors='replace')

    try:
        message = data[0:6]

        if (message == '$GNGGA'):
            #0 $GNGGA
            #1 gpstime
            #2,3 lat
            #4,5 lon
            #6 fixqual
            #7 satcount
            #...
            parts = data.split(',') # separate NMEA on commas
            # should check len(parts)
            latitude = decimalDegrees(parts[2], 2, parts[3]) # DDMM.mmm
            longitude = decimalDegrees(parts[4], 3, parts[5]) # DDDMM.mmm, decimal places in degree field, hemisphere
            fixtype = parts[6] # 4=RTK Fixed, 5=RTK Float

            print( str(latitude) + "," + str(longitude) + "," + str(fixtype))

    except Exception:
        print ("GPS:JUNK")


def closeout():
    gps.close()

    print ("Application closed!")
    exit()

print ("Application started!")

# Open GPS COM Port / TTY
gps = serial.Serial(GPS_SERIAL, baudrate = GPS_BAUD, timeout = 0.5)

if __name__ == "__main__":

    while running:
        try:
            getPositionData(gps)

        except KeyboardInterrupt:
            # Do a Ctrl-C in Shell Window, not stop signal
            print ("KeyboardInterrupt");
            running = False

        except:
            # You should do some error handling here...
            print ("Application error!")
            closeout()

closeout()


