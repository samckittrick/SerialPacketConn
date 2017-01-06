/* 
   Library for Packet-Based serial communication.

    Copyright (C) 2017 Scott McKittrick

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   
   Packet Structure:
   ---------------------------------------------------------
   | Overhead Byte | Data | Checksum Byte | Frame End Byte |
   ---------------------------------------------------------
   

   Created By: Scott McKittrick Dec. 31st 2016
   
 */

#ifndef SERIALPACKETCONN_H
#define SERIALPACKETCONN_H

#include <stdint.h>
//#include "Arduino.h"

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
