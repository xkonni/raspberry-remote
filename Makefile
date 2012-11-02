DESCRIPTION = "makefile"
LICENSE = "GPL"
VERSION = 1.0

CC      = g++
CFLAGS += -L/usr/local/lib
CFLAGS += -lwiringPi


default:
  $(CC) -Wall test.cpp RCSwitch.cpp -o test $(CFLAGS)

clean:
  rm -f test
