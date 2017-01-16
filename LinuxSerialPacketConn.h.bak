/* 
     Library for Packet-Based serial communications on Linux Machines
  
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
   
   Created By: Scott McKittrick Jan. 6 2017
   
*/

#ifndef LINUXSERIALPACKETCONN_H
#define LINUXSERIALPACKETCONN_H

#include "SerialPacketConn.h"

#include <termios.h>
#include <stdint.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fcntl.h>

class LinuxSerialPacketConn : public SerialPacketConn
{
 public:
  LinuxSerialPacketConn();
  int connect();
  int disconnect();
  const std::string getDeviceName();
  int setDeviceName(std::string name);
  int setBaudRate(int rate);
  const int getBaudRate();
  
 private:
  int fd;
  speed_t baudRate;
  struct termios oldtio;
  std::string devName;
  
  void writeBytes(uint8_t *buffer, int length);
  int readBytes(uint8_t *buffer, int length);
  void setConnAttr(struct termios *newtio);
};
#endif
