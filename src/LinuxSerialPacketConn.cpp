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

   Created By: Scott McKittrick, Jan. 6th, 2017
   
*/

#include "LinuxSerialPacketConn.h"
LinuxSerialPacketConn::LinuxSerialPacketConn()
{
  processBlock = true;
}

int LinuxSerialPacketConn::setDeviceName(const std::string name)
{
  devName.assign(name);
  return 0;
}

const std::string LinuxSerialPacketConn::getDeviceName()
{
  return std::string(devName);
}

int LinuxSerialPacketConn::setBaudRate(int rate)
{
  switch(rate)
    {
    case 9600:
      baudRate = B9600;
      return 0;
    case 19200:
      baudRate = B19200;
      return 0;
    case 38400:
      baudRate = B38400;
      return 0;
    default:
      return -1;
    }
}

const int LinuxSerialPacketConn::getBaudRate()
{
  switch(baudRate)
    {
    case B9600:
      return 9600;
    case B19200:
      return 19200;
    case B38400:
      return 38400;
    }
}

//Set up and open connection to specified serial device.
//When this function is complete, you should be able to use the file descriptor to read and write to the serial device.
//returns -1 on error
int LinuxSerialPacketConn::connect()
{

  //Set up required variables
  struct termios newtio;

  //Make sure the device has been selected
  if(devName.size() <= 0)
    {
      return -1;
    }
  //std::cout << "Opening File Descriptor";
  //Open the file Descriptor.
  fd = open(devName.c_str(), O_RDWR | O_NOCTTY);
  if(fd < 0)
    {
      return -1;
    }

  
  //Get device attributes to restore later
  if(tcgetattr(fd, &oldtio) < 0 )
    {
      return -1;
    }
  //std::cout << "Setting up connection";
  //set up connection configuration
  memset(&newtio, 0, sizeof(newtio));
  setConnAttr(&newtio);
  //std::cout << "Opening File Descriptor";
  if(tcsetattr(fd, TCSANOW, &newtio) < 0)
    {
      return -1;
    }

  return fd;

}

void LinuxSerialPacketConn::setConnAttr(struct termios *newtio)
{

  //Clean the new tio struct
  //Size of on a pointer? Need to verify this
  //memset(&newtio, 0, sizeof(newtio));
  //Set baud rate
  cfsetispeed(newtio, baudRate);
  cfsetospeed(newtio, baudRate);

  //newtio.c_iflag is already set to 0
  ///newtio.c_oflag is already set to 0
  
  //Set the port to 8N1 protocol
  //8 bit data with no parity bit and 1 stop bit;
  newtio->c_cflag &= ~PARENB;
  newtio->c_cflag &= ~CSTOPB;
  newtio->c_cflag &= ~CSIZE;

  //Character size mask to 8 bits
  newtio->c_cflag |= CS8;

  //Arduino doesn't have RTS/CTS lines 
  //disable hardware flow control
  newtio->c_cflag &= ~CRTSCTS;

  //enable receiver
  newtio->c_cflag |= CREAD;
 
  //Arduino has no modem control lines
  newtio->c_cflag |= CLOCAL;

  //Turn off several features for the local flags
  newtio->c_lflag &= ~(ICANON | ISIG | ECHO | IEXTEN);

  //Configuring vmin and vtime
  //Further information can be found at:
  //http://unixwiz.net/techtips/termios-vmin-vtime.html
  //Starting with vmin = 1 and vtime = 1
  //Wait for at least 1 character and hopefully receive each character immediately after
  //ToDo: Make this something that can be customized since different values may be more efficient depending on
  //on whether or not we want to seat commands with parameters or streams of data.
  newtio->c_cc[VTIME] = 1;
  newtio->c_cc[VMIN] = 0;
  

}

//This function frees the file descriptor.
int LinuxSerialPacketConn::disconnect()
{
  
  tcsetattr(fd, TCSANOW, &oldtio);

  if(close(fd) == 0)
    {
      return 0;
    }
  else
    {
      return -1;
    }
}

int LinuxSerialPacketConn::recvData(uint8_t *data, int len, int timeout)
{
  //may need a timeout here.
  //Read in as much data as is available until we reach a max size, or a frame end character
  bool packetComplete = false;
  uint8_t buffer;
  int recvCount = 0;
  uint8_t packet[MAXCOBSPACKETLEN];

  //Set timeout
  time_t beginTime = time(NULL);
  while((recvCount < MAXCOBSPACKETLEN) && !packetComplete && (difftime(time(NULL), beginTime) < timeout))
    {
      //std::cout << "Received data. ";
      if(read(fd, &buffer, 1) >0)
	{
	  packet[recvCount] = buffer;
	  //printf("%02X\n", buffer);
	  recvCount++;
	  //Serial.write(0xF1);
        
	  if(buffer  == COBS_FRAMEEND)
	    {
	      packetComplete = true;
	      //Serial.write(0xF2);
	    }
	}
      //recvCount++;
    }

  if(!packetComplete)
    {
      return -1;
    }


  //Allocate memory for the data
  uint8_t processed[dataLenFromPacketLen(recvCount)];
  int processResult = processPacket(packet, recvCount, processed, dataLenFromPacketLen(recvCount));

  recvCount = 0;
  if((processResult > 0) && (processResult <= len))
    {
      for(int i = 0; i < len; i++)
	{
	  data[i] = processed[i];
	}
      return processResult;
    }
  else
    {
      return -1;
    }
}

int LinuxSerialPacketConn::sendMessage(const uint8_t *data, int dataLength)
{
  uint8_t sPacket[packetLenFromDataLen(dataLength)];
  int packetLen = buildPacket(data, dataLength, sPacket, packetLenFromDataLen(dataLength));
  write(fd, sPacket, packetLen);
}
