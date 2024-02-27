# C94M8P page

I created these over several conversations over the uBlox Support Forum.

If you found this useful, or it saved you many hours of work, consider contributing to my efforts
  *  https://paypal.me/cliveone  
  *  https://www.venmo.com/u/cliveone

Picture of board showing J8 Header Orientation and Ground pins

![alt text](c94-m8p-d-board.jpg?raw=true)

ASCII Text Image/Representation of J8 Header

![alt text](c94-m8p-d-hdr.jpg?raw=true)

ASCII Text Image of underlying circuit connectivity from schematic. AND combines normally-high signalling outputs.

![alt text](c94-m8p-d-logic.jpg?raw=true)

Micro Controller Connectivity (CMOS 3V Compatible)

![alt text](c94-m8p-d-micro.jpg?raw=true)

### Suggested Reading..
  *  https://portal.u-blox.com/s/question/0D52p0000AzKxWUCQ0/c94m8p1-pixhawk-interface
  *  https://portal.u-blox.com/s/question/0D52p0000E7qXkKCQU/the-c94m8p-green-led-never-comes-on-i-have-done-all-of-the-setups-and-the-fix-mode-show-time-included-is-picture-off-all-i-know-the-config-file-was-ask-to-be-saved-on-another-unit-but-this-one-i-had-to-manually-goto-the-config-and-save-it
  *  https://portal.u-blox.com/s/question/0D52p00009VxbbaCAB/problems-connecting-c94m8p-rover-to-an-arduino-due
  *  https://portal.u-blox.com/s/question/0D52p00008HKCwyCAH/c94m8p2-settings-to-use-j8-connector-for-external-communication
  *  https://portal.u-blox.com/s/question/0D52p00008HKD52CAH/c94m8p-rover-ubx-protocoll-send-by-radio-to-base-ubx-data-at-which-j8-jumper
  *  https://www.google.com/search?q=site%3Aportal.u-blox.com+clive1+j8.10


### The DENSA Summary

If you're doing integration with an MCU, consider OTHER BOARDS. If you insist on using this board, please try to recruit team members with >= 100 IQ who can solve puzzles.

The primarly dilema here is that the host should connect via USB, and that the UART functionality is a shared resource with complex plumbing. The board is designed as a functional demo, not an integration platform.

  *  The RS232, RADIO and NEO-M8P share ONE UART

  *  The RS232 primarily connects to the RADIO, and allows end-to-end communication with two boards. The RS232 doesn't connect with the local NEO-M8P. You can test viablility using a terminal as the prescribed baud rate at each end, and type messages to yourself. You can configure the remote NEO-M8P via the local RS232, but not the local NEO-M8P.
   
  *  The NEO-M8P is connected to the RADIO, so that in a BASE Configuration it sends RTCM3 packets ONLY to the ROVER
     
  *  The ROVER should be quiet and LISTEN. Output via the UART will disrupt the RADIO connection. The RADIO should be receiving RTCM3 packets from the BASE. ie 1005, 1077, 1087, 1127, 1230
    
  *  The board has a schematic, and I've diagramed the J8 connection and associated tap points. An MCU can observe traffic on the UART, RADIO and RS233 at various taps. Signals can also be injected at specific points, but remember this is a SHARED resource. The AND gates allow the mixing of Serial UART signals that are NORMALLY HIGH at IDLE.


Boards from Ardusimple and SparkFun might make significantly better choices for your RTK integration project.

