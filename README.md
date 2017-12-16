# Raspberry Pi Remote

## About
Control your remote power sockets with a raspberry pi.
This project includes a web interface.

There's also a powerswitch branch, if you want to imitate a ConnAir remote
station on the raspberry and use the nice android app from
http://power-switch.eu

And in case your Raspberry is running Windows10, check
https://github.com/Panzenbaby/Raspberry-Remote-for-Windows-10-IoT-Core

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
*  Switch on single socket: `./send 1 00001 1 1`
*  Switch on multiple sockets: `./send 1 00001 1 1 00001 2 1 00001 3 1`

### Options
* `-b`, `--binary`: Use binary socket numbering instead of the common "only one switch up"-numbering. See [Binary Mode](#binary-mode) for further details.
* `-p X`, `--pin=X` (X=pin number): Sets the pin number to use. Default is 0 in normal mode and 17 in [user mode](#user-mode).
* `-u`, `--user`: Run in user mode. This mode does not need root permissions, but the GPIO pin has to be exported beforehand using the `gpio` command. See [User Mode](#user-mode) for further details.
* `-s`, `--silent`: Disables all text output except for error messages.
* `-h`, `--help`: Display help.

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

## User Mode
Use this mode if you want to use `send` without root permission. The pin must be exported with the `gpio` utility beforehand because it will be used via the `/sys/class/gpio` interface. The command for the default pin is `gpio export 17 out`. The user must be a member of the *gpio* group to access exported gpio pins!.  
**Important Note:** pin numbering is different in this mode! While wiringPi uses its own numbering scheme in default mode, this mode requires the native Broadcom GPIO numbers (the default port 0 is 17 in this mode). See the [wiringPi documentation](http://wiringpi.com/pins/) for further details.

## Daemon
Use the daemon in combination with the webinterface
* Copy the files in webinterface in your http directory
* Edit ip address in config.php
* Edit the predefined setup of sockets in config.php
