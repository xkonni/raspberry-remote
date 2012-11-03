DESCRIPTION = "makefile"
LICENSE = "GPL"
VERSION = 1.0

CC      = g++
CFLAGS += -Iinclude
CFLAGS += -Llib
CFLAGS += -lwiringPi


default:
	$(CC) -Wall test.cpp RCSwitch.cpp -o test $(CFLAGS)

clean:
	rm -f test
