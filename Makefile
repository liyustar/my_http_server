VPATH = src include
CXXFLAGS = -I./include

httpd: httpd.o SocketUtility.o HttpUtility.o
	g++ $^ -o $@

httpd.o: SocketUtility.h HttpUtility.h

.PHONY: clean

clean:
	-rm *.o httpd


