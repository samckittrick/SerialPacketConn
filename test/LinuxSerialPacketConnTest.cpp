#include <iostream>
#include <stdio.h>
#include <stdint.h> 
#include <thread>
#include "LinuxSerialPacketConn.h"

using namespace std;

bool connected = false;

void packetReceived(const uint8_t *payload, int payloadLength)
{
  std::cout << "Packet Received: ";
  int i;
  for (i = 0; i < payloadLength; i++)
    {
      if (i > 0) printf(":");
      printf("%02X", payload[i]);
    }
  printf("\n");
  delete [] payload;
}

int main()
{
  LinuxSerialPacketConn conn;
  conn.setBaudRate(9600);
  conn.setDeviceName("/dev/ttyACM0");
  //conn.setPacketReceiver(packetReceived);

  cout << "Device name is " << conn.getDeviceName() << endl;
  cout << "Baud rate is " << conn.getBaudRate() << endl;

  cout << "Connecting to device...\n";
  if(conn.connect() < 0)
    {
      cout << "Error connecting to serial device\n";
      return 0;
    }
  else
    {
      cout << "Success.\n";
      connected = true;
    }

  usleep(1000);

  //thread t1(serialProcessThread, &conn);
  //t1.detach();
  std::string line;
  std::getline(std::cin, line);

  uint8_t message[] = { 0x11, 0x22, 0x00, 0x33 };

  conn.sendMessage(message, 4);
  uint8_t buffer[MAXDATALEN];
  int datalen = conn.recvData(buffer, MAXDATALEN);
  std::cout << "Packet Received: ";
  int i;
  for (i = 0; i < datalen; i++)
    {
      if (i > 0) printf(":");
      printf("%02X", buffer[i]);
    }
  printf("\n");

  //std::string line; 
  std::getline(std::cin, line);

  connected = false;
  conn.disconnect();
  //  t1.join();
  return 0;

}

  
