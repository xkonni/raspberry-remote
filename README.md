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


## Daemon
Use the daemon in combination with the webinterface
* Copy the files in webinterface in your http directory
* Edit ip address in config.php
* Edit the predefined setup of sockets in config.php
