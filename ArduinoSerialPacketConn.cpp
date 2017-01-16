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

void ArduinoSerialPacketConn::writeBytes(byte *buffer, int length)
{
	Serial.write(buffer, length);
}

int ArduinoSerialPacketConn::readBytes(byte *buffer, int len)
{
  if(Serial.available())
    {
      return Serial.readBytes(buffer, len);
    }
  else
    return 0;
}
