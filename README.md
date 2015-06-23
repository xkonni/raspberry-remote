# Raspberry Pi Remote

## About
Control your remote power sockets with a raspberry pi.
This project includes a web interface.

## Credits
* sui li for creating RCSwitch for the arduino
* r10r for porting RCSwitch to the raspberry
* x3 for creating homecontrol4me

## Required Hardware
* Raspberry Pi
* 434 Mhz transmitter
* Some radio controlled power sockets

## Setup
* Install wiringPi. See https://projects.drogon.net/raspberry-pi/wiringpi/
* Checkout this project
* Compile it
* Connect pin11 of the raspberry to the transmitter

## Usage
Try if all is working with the send program
*  Switch on single socket: `./send.cpp 00001 1 1`

Pass the `-b`-option to use binary socket numbering instead of the common "only one switch up"-numbering. See [Binary Mode](#binary-mode) for further details.

## Binary Mode
Most sockets available for purchase use the following numbering scheme:

no. | address
--- | -------
  A |   10000
  B |   01000
  C |   00100
  D |   00010
  E |   00001
  
Of course, this doesn't make much sense, because it limits the maximum of supported sockets to 5 (or 6, if 00000 is included), and is less intuitive. Using real binary numbering would increase the limit of supported sockets per system to 31, and be more intutive. In binary mode, the sockets need to be numbered as below:

no. | address
--- | -------
  1 |   00001
  2 |   00010
  3 |   00011
  4 |   00100
  5 |   00101
  8 |   01000
 16 |   10000
 31 |   11111
 
Note that you need to configure your sockets to this kind of numbering to use this feature. This often includes that the dedicated remote that gets shipped with the sockets often is rendered useless, since it only supports the former way of numbering.
  
## Daemon
Use the daemon in combination with the webinterface
* Copy the files in webinterface in your http directory
* Edit ip address in config.php
* Edit the predefined setup of sockets in config.php
