DESCRIPTION = "RCSwitch on Raspberry Pi"
LICENSE = "GPL"
VERSION = 1.0

CC      = g++
CFLAGS += -Iinclude
CFLAGS += -Llib
CFLAGS += -lwiringPi

default: daemon

daemon: daemon.cpp
	$(CC) -Wall daemon.cpp RCSwitch.cpp -o daemon $(CFLAGS)

test: test.cpp
	$(CC) -Wall test.cpp RCSwitch.cpp -o test $(CFLAGS)

clean:
	rm -f test daemon
