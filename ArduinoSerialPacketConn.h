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
 #ifndef ARDUINOSERIALPACKETCONN_H
 #define ARDUINOSERIALPACKETCONN_H
 #include "SerialPacketConn.h"
 #include "Arduino.h"
 
class ArduinoSerialPacketConn : public SerialPacketConn
{
	public:
		using SerialPacketConn::SerialPacketConn;
		void connect();
		void disconnect();

	private:
	  //Functions to be overriden by subclass for reading and writing to the serail device
	int readBytes(byte *buffer, int len); //Read bytes from serial
	void writeBytes(byte *buffer, int length); //write bytes to serial
};

 #endif
