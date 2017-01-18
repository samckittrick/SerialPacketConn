/*
  Library for Packet-Based serial communications.

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
*/

#include "SerialPacketConn.h"
SerialPacketConn::SerialPacketConn()
{
}

int SerialPacketConn::processPacket(uint8_t *packet, int packetLen, uint8_t *data, int dataLen)
{
	  //Decode packet
	  uint8_t payload[MAXPAYLOADLEN];
	  int payloadLen;
      payloadLen = COBUnStuff(packet, packetLen, payload);
	  if(payloadLen == -1)
		  return -1;
	  //Verify the checksum here. The result should be 0 if the checksum matches
      bool result = calculateChecksum(payload, payloadLen);
	  if(result != 0)
		  return -1;
	  if((payloadLen -1)  > dataLen)
		return -1;
	  for(unsigned short i = 0; i < payloadLen-1; i++)
	    {
	      data[i] = payload[i];
	    }
	  return payloadLen-1;
}


//Send a packet on the serial line
int SerialPacketConn::buildPacket(const uint8_t *data, int dataLength, uint8_t *packetBuffer, int packetLen)
{
  if(dataLength > MAXDATALEN)
    {
      return -1;
    }

  //Create the checksum here
  uint8_t checksum  = calculateChecksum(data, dataLength);
  //insert the checksum into the data.
  uint8_t payload[dataLength + 1];
  for(unsigned short i = 0; i < dataLength; i++)
    {
      payload[i] = data[i];
    }
  payload[dataLength] = checksum;

  //Encode the packet and send it.
  return packetLen = COBStuff(payload, dataLength + 1, packetBuffer);
}

//Consistent Overhead Byte Stuffing
int SerialPacketConn::COBStuff(uint8_t *buffer, int length, uint8_t *dest)
{
  if(length > MAXPAYLOADLEN)
    {
      return -1;
    }
  
  uint8_t *lastZero = dest++;
  uint8_t *end = buffer + length;
  uint8_t count = 0x01;
  int destSize = 1;
  
  while(buffer < end)
    {
      if(*buffer == COBS_FRAMEEND)
	{
	  *lastZero = count;
	  lastZero = dest;
	  count = 0x01;
	  buffer;
	}
      else
	{
	  *dest = *buffer;
	  count++;
	}
      dest++;
      buffer++;
      destSize++;
    }
  *dest = COBS_FRAMEEND;
  destSize++;
  *lastZero = count;
  return destSize;
  
}

//Consistent Overhead Byte UnStuffing
int SerialPacketConn::COBUnStuff(uint8_t *buffer, int length, uint8_t *dest)
{
  if(length > MAXCOBSPACKETLEN)
    {
      return -1;
    }

  
  //uint8_t *current = buffer+1;
  uint8_t *nextZero = buffer + *buffer;
  uint8_t *end = buffer + (length-1);
  unsigned short destSize = 0; 
  buffer++;
  
  while(buffer < end)
    {
      if(buffer == nextZero)
	{
	  nextZero = buffer + *buffer;
	  *dest = COBS_FRAMEEND;
	}
      else
	{
	  *dest = *buffer;
	}
      dest++;
      buffer++;
      destSize++;
    }
  return destSize;
}
  

//Calculate checksum using CRC8 algorithm
uint8_t SerialPacketConn::calculateChecksum(const uint8_t *data, int len)
{
  //#ifdef CHECKSUM
#define GENPOLYNOMIAL 0xD5
  uint8_t csum = 0x00;
  while(len--)
    {
      csum ^= *data++;
      for(uint8_t i = 0; i < 8; i++)
	{
	  if((csum & 0x80) != 0)
	    {
	      csum = (uint8_t)((csum << 1) ^ GENPOLYNOMIAL);
	    }
	  else
	    {
	      csum <<= 1;
	    }
	}
    }
  return csum;
  //#endif
    //return 0x00;
  //#endif
}
