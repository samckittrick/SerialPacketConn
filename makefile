CC=g++
CFLAGS=-std=c++11
INCLUDE=-Iinclude/
BUILDDIR=build

LinuxSerialPacketConn.a: $(BUILDDIR)/SerialPacketConn.o $(BUILDDIR)/LinuxSerialPacketConn.o
	ar cr $(BUILDDIR)/$@ $(BUILDDIR)/*
	rm $(BUILDDIR)/*.o

$(BUILDDIR)/LinuxSerialPacketConn.o: $(BUILDDIR)/SerialPacketConn.o src/LinuxSerialPacketConn.cpp include/LinuxSerialPacketConn.h
	$(CC) $(CFLAGS) $(INCLUDE) -c src/LinuxSerialPacketConn.cpp -o $@

$(BUILDDIR)/SerialPacketConn.o: src/SerialPacketConn.cpp include/SerialPacketConn.h
	$(CC) $(CFLAGS) $(INCLUDE) -c SerialPacketConn.cpp -o $@

clean:
	rm build/*
