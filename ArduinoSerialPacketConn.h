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
