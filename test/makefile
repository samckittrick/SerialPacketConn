CC=g++
CFLAGS=-std=c++11
INCLUDE=-I../include


all: JavaTest LinuxSerialPacketConnTest

LinuxSerialPacketConnTest: LinuxSerialPacketConn.o SerialPacketConn.o LinuxSerialPacketConnTest.cpp
	$(CC) $(CFLAGS) -pthread $(INCLUDE) LinuxSerialPacketConn.o SerialPacketConn.o LinuxSerialPacketConnTest.cpp  -o LinuxSerialPacketConnTest 

LinuxSerialPacketConn.o: ../src/LinuxSerialPacketConn.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c ../src/LinuxSerialPacketConn.cpp

SerialPacketConn.o: ../src/SerialPacketConn.cpp 
	$(CC) $(CFLAGS) $(INCLUDE) -c ../src/SerialPacketConn.cpp



#Build Java Test for arduino
JavaTest:
	javac SerialPacketConnTest.java

clean:
	rm -f *.class *.o LinuxSerialPacketConnTest
