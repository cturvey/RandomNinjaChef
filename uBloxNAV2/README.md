# NAV2 Related stuff

https://content.u-blox.com/sites/default/files/ZED-F9P-FW100-HPG130_RN_UBX-21047459.pdf#page=6

## Secondary output

A complete set of messages based on a navigation solution computed as a GNSS standalone
receiver is available at the same update rate as the primary output. These messages are available
both in NMEA and UBX protocols.

These messages can be configured to restrict the use to only GPS satellites which have integrity
monitoring messages. The resulting secondary output messages can be used as a check against
potential software faults in the correction service or RTK software in the system. The secondary
output is also a useful tool during development.

By default, the secondary output is disabled.
