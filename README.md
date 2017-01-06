
Classes for Packet-Based serial communication.
 
This library was inspired by:
https://github.com/bakercp/PacketSerial

- Features:
  - Serial Packet-Based Communication
  - COBS Encoding
  - Packet Integrity Checks

* Possible Future Features:
  * Retry Logic
  * Reliable vs Unreliable Packets
  * Multipacket Messages
  
 **Packet Structure:**
  ```
  ---------------------------------------------------------
  | Overhead Byte | Data | Checksum Byte | Frame End Byte |
  ---------------------------------------------------------
  ```
  
**The SerialPacketConn Class**

   This is an abstract class intended to provide the functions for creating and managing packets sent accross the serial line. It defines but does not implement functions for the actual setup/teardown and read/write operations on the serial device. To customize for the device it will be running on, create a subclass that implements the following functions:

```
  virtual void connect() = 0; //Open Connection
  virtual void disconnect() = 0; //Close Connection
  virtual int readBytes(uint8_t *buffer, int len) = 0; //Read bytes from serial
  virtual void writeBytes(uint8_t *buffer, int length) = 0; //write bytes to serial
```

**The ArduinoSerialPacketConn Class**

   This is an Arduino specific subclass of the SerialPacketConn class. It can be used as a library on the Arduino.
