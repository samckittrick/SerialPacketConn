#include <iostream>
#include <stdio.h>
#include <stdint.h> 
#include <thread>
#include "LinuxSerialPacketConn.h"

using namespace std;

bool connected = false;

void serialProcessThread(LinuxSerialPacketConn *conn)
{
  cout << "Thread Started\n";
  while(connected)
    {
      //cout << "Processing\n";
      conn->process();
      //cout << "Processing done\n";
      usleep(1000);
    }
}

void packetReceived(const uint8_t *payload, int payloadLength)
{
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
  conn.setPacketReceiver(packetReceived);

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

  thread t1(serialProcessThread, &conn);
  //t1.detach();

  uint8_t message[] = { 0x11, 0x22, 0x00, 0x33 };

  conn.sendMessage(message, 4);

  std::string line; 
  std::getline(std::cin, line);

  uint8_t message2[] = { 0x00, 0x02, 0x30, 0xFF, 0xAD };
  conn.sendMessage(message2, 5);

  std::getline(std::cin, line);

  connected = false;
  t1.join();
  return 0;

}

  
