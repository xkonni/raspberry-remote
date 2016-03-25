/*
 Usage: ./send    <command>
 SystemCodeType is 1 for default and 2 for switches with 10 Bits 123456ABCD
 Command is 0 for OFF and 1 for ON
 */

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

	/*
	output PIN is hardcoded for testing purposes
	see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
	for pin mapping of the raspberry pi GPIO connector
	*/
	int systemCodeType = atoi(argv[1]);
 	char* systemCode = argv[2];
	int unitCode = atoi(argv[3]);
	int command = atoi(argv[4]);
	int PIN = 0; //GPIO-Pin 17
	if (wiringPiSetup () == -1) return 1;
 	printf("sending systemCodeType[%i] systemCode[%s] unitCode[%i] command[%i] ...\n", systemCodeType, systemCode, unitCode, command);
 	RCSwitch mySwitch = RCSwitch();
 	printf("defining transmit PIN[%i] ... ",PIN);
 	mySwitch.enableTransmit(PIN);
 	printf("success\n");
 	printf("computing system Code Type ...\n");
 	switch(systemCodeType)
		{
		 case 1:
			{
			 printf("Switching \"default\" system[%s] unit[%i] ... ", systemCode, unitCode);
			 switch(command)
			 	{
				 case 0:
					{
					printf("off\n");
					mySwitch.switchOffBinary(systemCode, unitCode);
					break;
					}
				 case 1:
					{
					printf("on\n");
					mySwitch.switchOnBinary(systemCode, unitCode);
					break;
					}
				 default:
					{
					printf("command[%i] is unsupported\n", command);
					return -1;
					}
			 	}
			break;
			}
		 case 2:
			{
			printf("computing systemcode for Intertechno Type B house[%s] unit[%i] ... ",systemCode, unitCode);
			char pSystemCode[14];
			switch(atoi(systemCode))
				{
				// house/family code A=1 - P=16
				case 1:     { printf("1/A ... ");	    strcpy(pSystemCode,"0000");	break; }
				case 2:     { printf("2/B ... ");	    strcpy(pSystemCode,"F000");	break; }
				case 3:	    { printf("3/C ... ");	    strcpy(pSystemCode,"0F00");	break; }
				case 4:     { printf("4/D ... ");	    strcpy(pSystemCode,"FF00");	break; }
				case 5:	    { printf("5/E ... ");	    strcpy(pSystemCode,"00F0");	break; }
				case 6:     { printf("6/F ... ");	    strcpy(pSystemCode,"F0F0");	break; }
				case 7:	    { printf("7/G ... ");	    strcpy(pSystemCode,"0FF0");	break; }
				case 8:     { printf("8/H ... ");	    strcpy(pSystemCode,"FFF0");	break; }
				case 9:	    { printf("9/I ... ");	    strcpy(pSystemCode,"000F");	break; }
				case 10:    { printf("10/J ... ");	    strcpy(pSystemCode,"F00F");	break; }
				case 11:	{ printf("11/K ... ");	    strcpy(pSystemCode,"0F0F");	break; }
				case 12:    { printf("12/L ... ");	    strcpy(pSystemCode,"FF0F");	break; }
				case 13:	{ printf("13/M ... ");	    strcpy(pSystemCode,"00FF");	break; }
				case 14:    { printf("14/N ... ");	    strcpy(pSystemCode,"F0FF");	break; }
				case 15:	{ printf("15/O ... ");	    strcpy(pSystemCode,"0FFF");	break; }
				case 16:    { printf("16/P ... ");	    strcpy(pSystemCode,"FFFF");	break; }
				default:
					{
					printf("systemCode[%s] is unsupported\n", systemCode);
					return -1;
					}
				}
			printf("got systemCode\n");
			switch(unitCode)
			{
			// unit/group code 01-16
				case 1:     { printf("1 ... ");	    strcat(pSystemCode,"0000");	break; }
				case 2:     { printf("2 ... ");	    strcat(pSystemCode,"F000");	break; }
				case 3:	    { printf("3 ... ");	    strcat(pSystemCode,"0F00");	break; }
				case 4:     { printf("4 ... ");	    strcat(pSystemCode,"FF00");	break; }
				case 5:	    { printf("5 ... ");	    strcat(pSystemCode,"00F0");	break; }
				case 6:     { printf("6 ... ");	    strcat(pSystemCode,"F0F0");	break; }
				case 7:	    { printf("7 ... ");	    strcat(pSystemCode,"0FF0");	break; }
				case 8:     { printf("8 ... ");	    strcat(pSystemCode,"FFF0");	break; }
				case 9:	    { printf("9 ... ");	    strcat(pSystemCode,"000F");	break; }
				case 10:    { printf("10 ... ");	    strcat(pSystemCode,"F00F");	break; }
				case 11:	{ printf("11 ... ");	    strcat(pSystemCode,"0F0F");	break; }
				case 12:    { printf("12 ... ");	    strcat(pSystemCode,"FF0F");	break; }
				case 13:	{ printf("13 ... ");	    strcat(pSystemCode,"00FF");	break; }
				case 14:    { printf("14 ... ");	    strcat(pSystemCode,"F0FF");	break; }
				case 15:	{ printf("15 ... ");	    strcat(pSystemCode,"0FFF");	break; }
				case 16:    { printf("16 ... ");	    strcat(pSystemCode,"FFFF");	break; }
				default:
				{
					printf("unitCode[%i] is unsupported\n", unitCode);
        	   	 return -1;
				}
		 }
		strcat(pSystemCode,"0F"); // mandatory bits
        switch(command)
        {
             case 0:
                 {
                 strcat(pSystemCode,"F0");
                 mySwitch.sendTriState(pSystemCode);
                 printf("sent TriState signal: pSystemCode[%s]\n",pSystemCode);
                 break;
                 }
             case 1:
                 {
                 strcat(pSystemCode,"FF");
                 mySwitch.sendTriState(pSystemCode);
                 printf("sent TriState signal: pSystemCode[%s]\n",pSystemCode);
                 break;
                 }
             default:
                 {
                 printf("command[%i] is unsupported\n", command);
                 return -1;
                 }
            }
            break;
	}
        default:
            {
                printf("command sequence unknown, aborting!\n");
                return -1;
            }
		}
 return 0;
=======
#include <string>
#include <getopt.h>
//#include <iostream>

void printUsage() {
    printf("This is rasperry remote, an application to control remote plugs with the\nRaspberry Pi.\n");
    printf("Based on RCSwitch and wiringPi. See github.com/xkonni/raspberry-remote for\nfurther reference.\n");
    printf("Usage: \n sudo send [options] <systemCode> <unitCode> [<systemCode> <unitCode>] <command>\n or: sudo send -h\n\n");
    printf("Options:\n\n");
    printf(" -b, --binary:\n");
    printf("   Switches the instance to binary mode.\n");
    printf("   Binary mode means, that instead numbering the sockets by 00001 00010 00100\n");
    printf("   01000 10000, the sockets are numbered in real binary numbers as following:\n");
    printf("   00001 00010 00011 00100 00101 00110 and so on.\n");
    printf("   This means that your sockets need to be setup in this manner, which often\n");
    printf("   includes that the dedicated remote is rendered useless, but more than\n");
    printf("   6 sockets are supported.\n\n");
    printf(" -h, --help:\n");
    printf("   displays this help\n\n");
    printf(" -p X, --pin=X\n");
    printf("   Sets the pin to use to communicate with the sender.\n");
    printf("   Important note: when running as root, pin numbers are wiringPi numbers while\n");
    printf("   in user mode (-u, --user) pin numbers are BCM_GPIO numbers.\n");
    printf("   See http://wiringpi.com/pins/ for details.\n");
    printf("   Default: 0 in normal mode, 17 in user mode\n\n");
    printf(" -s, --silent:\n");
    printf("   Don't print any text, except for errors\n\n");
    printf(" -u, --user:\n");
    printf("   Switches the instance to user mode.\n");
    printf("   In this mode this program does not need root privileges. It is required to\n");
    printf("   export the GPIO pin using the gpio utility. Note that pin numbers are\n");
    printf("   BCM_GPIO numbers in this mode! the export command for the default port is\n");
    printf("   \"gpio export 17 out\". For more information about port numbering see\n");
    printf("   http://wiringpi.com/pins/.\n\n");
}

int main(int argc, char *argv[]) {
    bool silentMode = false;
    bool binaryMode = false;
    bool userMode = false;
    int pin = 0;
    int controlArgCount = 0;
    char *systemCode;
    int unitCode;
    int command;
    bool multiMode;

    int c;
    while (1) {
        static struct option long_options[] =
            {
              {"binary", no_argument, 0, 'b'},
              {"help", no_argument, 0, 'h'},
              {"pin", required_argument, 0, 'p'},
              {"silent", no_argument, 0, 's'},
              {"user", no_argument, 0, 'u'},
              0
            };
        int option_index = 0;

        c = getopt_long (argc, argv, "bhp:su",long_options, &option_index);
        /* Detect the end of the options. */
        if (c == -1)
            break;
<<<<<<< HEAD

        switch (c) {
            case 'b':
                binaryMode = true;
                break;
            case 'p':
                pin = atoi(optarg);
                break;
            case 's':
                silentMode = true;
                break;
            case 'u':
                userMode = true;
                break;
            case 'h':
                printUsage();
                return 0;
        }
    }

=======

        switch (c) {
            case 'b':
                binaryMode = true;
                break;
            case 'p':
                pin = atoi(optarg);
                break;
            case 's':
                silentMode = true;
                break;
            case 'u':
                userMode = true;
                break;
            case 'h':
                printUsage();
                return 0;
        }
    }

>>>>>>> xkonni/master
    // When started in user mode, wiringPiSetupSys() is used. In this mode
    // wiringPi uses the /sys/class/gpio interface and the GPIO pin are numbered
    // by the native Broadcom GPIO numbers, where wiringPi-number 0 translates to
    // bcm-number 17... so use this as default if no pin was set with -p
    // reference:
    //   http://wiringpi.com/reference/setup/
    //   http://wiringpi.com/pins/
    if (pin == 0 && userMode) {
      pin = 17;
    }

    controlArgCount = argc - optind;
    // we need at least 3 args: systemCode, unitCode and command
    if (controlArgCount >= 3) {
        if (!silentMode) {
            if (binaryMode) {
                printf("operating in binary mode\n");
            }
            if (userMode) {
                printf("operating in user mode\n");
            }
            printf("using pin %d\n", pin);
        }

        char *controlArgs[controlArgCount];
        int i = 0;
        while (optind < argc && i < controlArgCount) {
            controlArgs[i] = argv[optind++];
            i++;
        }

        multiMode = controlArgCount > 3;

        int numberOfActuators = (controlArgCount - 1) / 2;
        // check if there are enough arguments supplied, we need (numberOfActuators * 2) + 1

        if (controlArgCount != (numberOfActuators * 2) + 1) {
          printf("invalid set of control arguments\nuse <systemCode> <unitCode> <command> or\n");
          printf("<systemCode> <unitCode> [<systemCode> <unitCode>...] <command>\n");
          return 1;
<<<<<<< HEAD
        }

        if (multiMode && !silentMode) {
            printf("multi mode\n");
        }

        command = atoi(controlArgs[controlArgCount - 1]);
        if (userMode) {
            if (wiringPiSetupSys() == -1) return 1;
        } else {
            if (wiringPiSetup() == -1) return 1;
=======
>>>>>>> xkonni/master
        }
        piHiPri(20);
        RCSwitch mySwitch = RCSwitch();
        mySwitch.setPulseLength(300);
        mySwitch.enableTransmit(pin);

<<<<<<< HEAD
        for (i = 0; i < numberOfActuators; i++) {
            int indexSystemCode = i * 2;
            int indexUnitCode = indexSystemCode + 1;
            systemCode = controlArgs[indexSystemCode];
            unitCode = atoi(controlArgs[indexUnitCode]);

            if (!silentMode) {
                printf("sending systemCode[%s] unitCode[%i] command[%i]\n", systemCode, unitCode, command);
            }
            if (binaryMode) {
                switch (command) {
                    case 1:
                        mySwitch.switchOnBinary(systemCode, unitCode);
                        break;
                    case 0:
                        mySwitch.switchOffBinary(systemCode, unitCode);
                        break;
                    default:
                        printf("command[%i] is unsupported\n", command);
                        printUsage();
                        if (!multiMode) {
                            return -1;
                        }
                }
            } else {
                switch (command) {
                    case 1:
                        mySwitch.switchOn(systemCode, unitCode);
                        break;
                    case 0:
                        mySwitch.switchOff(systemCode, unitCode);
                        break;
                    case 2:
                        // 00001 2 on binary coded
                        mySwitch.send("010101010001000101010001");
                        break;
                    case 3:
                        // 00001 2 on as TriState
                        mySwitch.sendTriState("FFFF0F0FFF0F");
                        break;
                    default:
                        printf("command[%i] is unsupported\n", command);
                        printUsage();
                        if (!multiMode) {
                            return -1;
                        }
                }
            }
        }
=======
        if (multiMode && !silentMode) {
            printf("multi mode\n");
        }

        command = atoi(controlArgs[controlArgCount - 1]);
        if (userMode) {
            if (wiringPiSetupSys() == -1) return 1;
        } else {
            if (wiringPiSetup() == -1) return 1;
        }
        piHiPri(20);
        RCSwitch mySwitch = RCSwitch();
        mySwitch.setPulseLength(300);
        mySwitch.enableTransmit(pin);

        for (i = 0; i < numberOfActuators; i++) {
            int indexSystemCode = i * 2;
            int indexUnitCode = indexSystemCode + 1;
            systemCode = controlArgs[indexSystemCode];
            unitCode = atoi(controlArgs[indexUnitCode]);

            if (!silentMode) {
                printf("sending systemCode[%s] unitCode[%i] command[%i]\n", systemCode, unitCode, command);
            }
            if (binaryMode) {
                switch (command) {
                    case 1:
                        mySwitch.switchOnBinary(systemCode, unitCode);
                        break;
                    case 0:
                        mySwitch.switchOffBinary(systemCode, unitCode);
                        break;
                    default:
                        printf("command[%i] is unsupported\n", command);
                        printUsage();
                        if (!multiMode) {
                            return -1;
                        }
                }
            } else {
                switch (command) {
                    case 1:
                        mySwitch.switchOn(systemCode, unitCode);
                        break;
                    case 0:
                        mySwitch.switchOff(systemCode, unitCode);
                        break;
                    case 2:
                        // 00001 2 on binary coded
                        mySwitch.send("010101010001000101010001");
                        break;
                    case 3:
                        // 00001 2 on as TriState
                        mySwitch.sendTriState("FFFF0F0FFF0F");
                        break;
                    default:
                        printf("command[%i] is unsupported\n", command);
                        printUsage();
                        if (!multiMode) {
                            return -1;
                        }
                }
            }
        }
>>>>>>> xkonni/master
        return 0;
    } else {
        printUsage();
        return -1;
<<<<<<< HEAD
>>>>>>> xkonni/master
=======
>>>>>>> xkonni/master
    }
