VPATH = src include
CXXFLAGS = -I./include

httpd: httpd.o SocketUtility.o HttpUtility.o HttpConfig.o HttpLog.o
	g++ $^ -o $@

httpd.o: SocketUtility.h HttpUtility.h HttpConfig.h HttpLog.h

.PHONY: clean

clean:
	-rm *.o httpd


