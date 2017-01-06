/* 
   Library for Packet-Based serial communication on arduino. This subclasses the generic SerialPacketConn
   Features:
   COBS Encoding
   Packet integrity checking -- CRC8 (Enable by defining CHECKSUM_CHECK
   Retry logic //ToDo
   Packet read timeout //ToDo
   
   Packet Structure:
   ---------------------------------------------------------
   | Overhead Byte | Data | Checksum Byte | Frame End Byte |
   ---------------------------------------------------------
   

   Created By: Scott McKittrick Dec. 31st 2016
   
 */
#include "ArduinoSerialPacketConn.h"

void ArduinoSerialPacketConn::connect()
{
  Serial.begin(baudRate);
}

void ArduinoSerialPacketConn::disconnect()
{
  Serial.end();
}

void ArduinoSerialPacketConn::writeBytes(byte *buffer, int length)
{
	Serial.write(buffer, length);
}

int ArduinoSerialPacketConn::readBytes(byte *buffer, int len)
{
  return Serial.readBytes(buffer, len);
}
