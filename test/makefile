CC=g++
CFLAGS=-std=c++11
INCLUDE=-I../ 


all: JavaTest LinuxSerialPacketConnTest

LinuxSerialPacketConnTest: LinuxSerialPacketConn.o SerialPacketConn.o
	$(CC) $(CFLAGS) $(INCLUDE) LinuxSerialPacketConn.o SerialPacketConn.o LinuxSerialPacketConnTest.cpp  -o LinuxSerialPacketConnTest 

LinuxSerialPacketConn.o: ../LinuxSerialPacketConn.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c ../LinuxSerialPacketConn.cpp

SerialPacketConn.o: ../SerialPacketConn.cpp 
	$(CC) $(CFLAGS) $(INCLUDE) -c ../SerialPacketConn.cpp



#Build Java Test for arduino
JavaTest:
	javac SerialPacketConnTest.java

clean:
	rm *.class *.o LinuxSerialPacketConnTest
