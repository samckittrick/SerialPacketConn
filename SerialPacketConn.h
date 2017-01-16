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
#define dataLenFromPacketLen(x) (x-3)
#define packetLenFromDataLen(x) (x+3)
#define COBS_FRAMEEND 0

//#define CHECKSUM

class SerialPacketConn
{
 public:
  SerialPacketConn();
  
  //Set up and tear down the serial connection
  virtual int connect() = 0; //Open Connection
  virtual int disconnect() = 0; //Close Connection

  //Public functions for working with the connection.
  virtual int sendMessage(const uint8_t *data, int dataLength) = 0;
  int processPacket(uint8_t *packet, int packetLen, uint8_t *data, int dataLen);
  int buildPacket(const uint8_t *data, int dataLen, uint8_t *packetBuffer, int packetLen); 
  
 protected:
  bool processBlock;
  
 private:
  

  //Functions for encoding and validating packet
  int COBStuff(uint8_t *buffer, int length, uint8_t *dest);
  int COBUnStuff(uint8_t *buffer, int length, uint8_t *dest);
  uint8_t calculateChecksum(const uint8_t *data, int len);

};

#endif
