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
   

   Created By: Scott McKittrick Dec. 31st 2016
   
 */
#include "ArduinoSerialPacketConn.h"

ArduinoSerialPacketConn::ArduinoSerialPacketConn(long speed)
{
  baudRate = speed;
  processBlock = false;
  recvCount = 0;
  myReceiver = nullptr;
}

//Sets the callback function
void ArduinoSerialPacketConn::setPacketReceiver(PacketReceiver receiver)
{
  myReceiver = receiver;
}

int ArduinoSerialPacketConn::connect()
{
  Serial.begin(baudRate);
  return 0;
}

int ArduinoSerialPacketConn::disconnect()
{
  Serial.end();
  return 0;
}

void ArduinoSerialPacketConn::readPacket()
{
	 //std::cout << "Process called\n";
  //Read in as much data as is available until we reach a max size, or a frame end character
  bool packetComplete = false;
  uint8_t buffer;
  while(Serial.available() && (recvCount < MAXCOBSPACKETLEN) && !packetComplete)
    {
      //std::cout << "Received data. ";
	  buffer = Serial.read();
      packet[recvCount] = buffer;
      //printf("%02X\n", buffer);
      recvCount++;
	  //Serial.write(0xF1);
	  
    if(buffer  == COBS_FRAMEEND)
	{
	  packetComplete = true;
	  //Serial.write(0xF2);
	}
      //recvCount++;
    }

  if(packetComplete)
    {
      //Serial.write(packet, recvCount);
      if((myReceiver != nullptr) ) //If someone is listening, notify them of the new packet
	  {
	    //Allocate memory for the data
	    uint8_t *data = new uint8_t[dataLenFromPacketLen(recvCount)];
		int processResult = processPacket(packet, recvCount, data, dataLenFromPacketLen(recvCount));
		if(processResult > 0)
		{
			//Serial.write(0xF4);
			(*myReceiver)(data, processResult);
		}
	  }
      recvCount = 0;
    }
}

int ArduinoSerialPacketConn::sendMessage(const uint8_t *data, int dataLength)
{
	uint8_t sPacket[packetLenFromDataLen(dataLength)];
	int packetLen = buildPacket(data, dataLength, sPacket, packetLenFromDataLen(dataLength));
	Serial.write(sPacket, packetLen);
}
