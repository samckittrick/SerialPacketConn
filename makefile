CC=g++
CFLAGS=-std=c++11
INCLUDE=-Iinclude/
BUILDDIR=build

LinuxSerialPacketConn.a: $(BUILDDIR)/SerialPacketConn.o $(BUILDDIR)/LinuxSerialPacketConn.o
	ar cr $(BUILDDIR)/$@ $(BUILDDIR)/*

$(BUILDDIR)/LinuxSerialPacketConn.o: src/LinuxSerialPacketConn.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c src/LinuxSerialPacketConn.cpp -o $@

$(BUILDDIR)/SerialPacketConn.o: src/SerialPacketConn.cpp 
	$(CC) $(CFLAGS) $(INCLUDE) -c src/SerialPacketConn.cpp -o $@

clean:
	rm build/*
