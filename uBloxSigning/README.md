Place holder for UBX-SEC-SIGN and UBX-SEC-ECSIGN examples

UBX-SEC-SIGN uses a salted-SHA-256 using a fixed, and dynamic seed front and back of specific messages. The message contains the class/id of the messaged being signed, and the fletcher checksum of the packet.

https://portal.u-blox.com/s/question/0D52p00008HKCkJCAX/how-to-verify-ubxsecsign

UBX-SEC-ECSIGN uses a SHA-256 to hash the transmission on a specific interface, less the UBX-SEC-ECSIGN message itself. You can verify the SHA-256 hash without knowing the signing keys. The hash and session id are signed using SECP192R1 Elliptic Curve with a private key, and you can authenticate the message using the public key. The primary vunerability here is if you know, or can recover, the private key. The presumed use case is that the private key is burned into the receiver's OTP during a vendor's production programming/test and not shared with the application side. The application only needs the public key, and can thwart replay attacks by using a unique session id.

The vendor should probably generate unique keys to bind a specific ZED-F9x or NEO-M9x to a specific board, although this may hamper the interchangability of parts/subsystems.
