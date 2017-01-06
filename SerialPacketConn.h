/* 
   Library for Packet-Based serial communication on arduino.
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

#ifndef SERIALPACKETCONN_H
#define SERIALPACKETCONN_H

#include <stdint.h>
#include "Arduino.h"

#define MAXCOBSPACKETLEN 257
#define MAXPAYLOADLEN  255
#define MAXDATALEN 254
#define COBS_FRAMEEND 0

//#define CHECKSUM

class SerialPacketConn
{
 public:
  //The payload will be a dynamically allocated byte array.
  //It will be the responsibility of the the application using this class to free the memory
  typedef void (*PacketReceiver)(const uint8_t *payload, int payloadLength);
  void setPacketReceiver(PacketReceiver receiver);
  
  //Set up and tear down the serial connection
  SerialPacketConn(long speed);
  virtual void connect() = 0; //Open Connection
  virtual void disconnect() = 0; //Close Connection

  //Public functions for working with the connection.
  int sendMessage(const uint8_t *data, int dataLength);
  void process();

 protected:
    long baudRate;
  
 private:
  PacketReceiver myReceiver;
  unsigned short recvCount;

  uint8_t  packet[MAXCOBSPACKETLEN];
  uint8_t  payload[MAXPAYLOADLEN];
  int payloadLen;
  
  //Functions to be overriden by subclass for reading and writing to the serail device
  virtual int readBytes(uint8_t *buffer, int len) = 0; //Read bytes from serial
  virtual void writeBytes(uint8_t *buffer, int length) = 0; //write bytes to serial
  

  //Functions for encoding and validating packet
  int COBStuff(uint8_t *buffer, int length, uint8_t *dest);
  int COBUnStuff(uint8_t *buffer, int length, uint8_t *dest);
  uint8_t calculateChecksum(const uint8_t *data, int len);

};

#endif
