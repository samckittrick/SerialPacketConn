
##Classes for Packet-Based serial communication.
 
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
  
##The SerialPacketConn Class

   This is an abstract class intended to provide the functions for creating and managing packets sent accross the serial line. It defines but does not implement functions for the actual setup/teardown and read/write operations on the serial device. To customize for the device it will be running on, create a subclass that implements the following functions:

```
  virtual void connect() = 0; //Open Connection
  virtual void disconnect() = 0; //Close Connection
  virtual int readBytes(uint8_t *buffer, int len) = 0; //Read bytes from serial
  virtual void writeBytes(uint8_t *buffer, int length) = 0; //write bytes to serial
```

##The ArduinoSerialPacketConn Class

   This is an Arduino specific subclass of the SerialPacketConn class. It can be used as a library on the Arduino.

**Example Usage**

```
#include <ArduinoSerialPacketConn.h>

ArduinoSerialPacketConn conn(9600);

void packetReceived(const byte* payload, int payloadLength)
{
  //Serial.write(payload, payloadLength);
  conn.sendMessage(payload, payloadLength);
  delete [] payload;
}

void setup() {
  // put your setup code here, to run once:
  conn.setPacketReceiver(&packetReceived);
  conn.connect();
}

void loop() {
  // put your main code here, to run repeatedly.
  conn.process();
}
```

##The LinuxSerialPacketConn Class
	
	This is a linux specific subclass of the SerialPacketConn class.
	
**Example Usage**
```
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <thread>
#include "LinuxSerialPacketConn.h"

using namespace std;

bool connected = false;

void packetReceived(const uint8_t *payload, int payloadLength)
{
  std::cout << "Packet Received: ";
  int i;
  for (i = 0; i < payloadLength; i++)
    {
      if (i > 0) printf(":");
      printf("%02X", payload[i]);
    }
  printf("\n");
  delete [] payload;
}

int main()
{
  LinuxSerialPacketConn conn;
  conn.setBaudRate(9600);
  conn.setDeviceName("/dev/ttyACM0");
  //conn.setPacketReceiver(packetReceived);

  cout << "Device name is " << conn.getDeviceName() << endl;
  cout << "Baud rate is " << conn.getBaudRate() << endl;

  cout << "Connecting to device...\n";
  if(conn.connect() < 0)
    {
      cout << "Error connecting to serial device\n";
      return 0;
    }
  else
    {
      cout << "Success.\n";
      connected = true;
    }

  usleep(1000);

  //thread t1(serialProcessThread, &conn);
  //t1.detach();
  std::string line;
  std::getline(std::cin, line);

  uint8_t message[] = { 0x11, 0x22, 0x00, 0x33 };

  conn.sendMessage(message, 4);
  uint8_t buffer[MAXDATALEN];
  int datalen = conn.recvData(buffer, MAXDATALEN);
  std::cout << "Packet Received: ";
  int i;
  for (i = 0; i < datalen; i++)
    {
      if (i > 0) printf(":");
      printf("%02X", buffer[i]);
    }
  printf("\n");

  //std::string line;
  std::getline(std::cin, line);

  connected = false;
  conn.disconnect();
  //  t1.join();
  return 0;

}
```
