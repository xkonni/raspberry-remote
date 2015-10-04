/*
 * Usage: ./send <systemCode> <unitCode> <command>
 * Command is 0 for OFF and 1 for ON
 */

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
//#include <iostream>

void printUsage() {
    printf("This is rasperry remote, an application to control remote plugs with the Raspberry Pi.\n");
    printf("Based on RCSwitch and wiringPi. See github.com/xkonni/raspberry-remote for further reference.\n");
    printf("Usage: \n sudo send [-b] <systemCode> <unitCode> <command> or:\n sudo send -h\n");
    printf("Where the -b argument switches the instance to binary mode, and the -h option displays this help.\n\n");
    printf("Binary mode means, that instead numbering the sockets by 00001 00010 00100 01000 10000, the sockets\n");
    printf("are numbered in real binary numbers as following: 00001 00010 00011 00100 00101 00110 and so on.\n");
    printf("This means that your sockets need to be setup in this manner, which often includes that the dedicated remote\n");
    printf("is rendered useless, but more than 6 sockets are supported.\n");
}

int main(int argc, char *argv[]) {
    /**
     * output PIN is hardcoded for testing purposes
     * see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     * for pin mapping of the raspberry pi GPIO connector
     */
    int PIN = 0;
    /**
     * using old numbering mode by default,
     * see RCSwitch.cpp for differences
     */
    bool binaryMode = false;
    char *systemCode;
    int unitCode;
    int command;

    if (argc > 5) {
        /**
         * no matter, which mode is used, at least 4 arguments are required:
         * 0: command name (send)
         * [
         * 1: systemCode
         * 2: unitCode
         * ]*n
         * 3: command
         * if there are less arguments, the help should be printed
         * and the application should terminate.
         */
        multipleSwitch(argc, *argv);
        return 0;
    }

    if (argc < 4) {
        /**
         * no matter, which mode is used, at least 4 arguments are required:
         * 0: command name (send)
         * 1: systemCode
         * 2: unitCode
         * 3: command
         * if there are less arguments, the help should be printed
         * and the application should terminate.
         */
        printUsage();
        return 1;
    }

    /**
     * This needs to stand after the check of how many arguments are passed,
     * because if there is only 1 argument passed, argv[1] is NULL, which
     * will result in an error, because a std::string can't be constructed
     * by a NULL value. Therefore it is important to terminate the application
     * if there are less than 2 arguments (in this case: less than 4 arguments)
     * passed.
     */
    std::string firstArgument = argv[1];

    if (firstArgument == "-b" or firstArgument == "--binary") {
        if (argc < 5) {
            /**
             * in binaryMode, 5 arguments are required:
             * 0: command name ('send')
             * 1: binary operator ('-b')
             * 2: systemCode
             * 3: unitCode
             * 4: command
             * if there are less arguments, the help should be printed,
             * and the application should terminate.
             */
            printUsage();
            return 1;
        }

        printf("operating in binary mode...\n");
        binaryMode = true;
        //position of data in input is now shifted by 1 because of '-b'-flag
        systemCode = argv[2];
        unitCode = atoi(argv[3]);
        command = atoi(argv[4]);
    } else if (firstArgument == "-h" or firstArgument == "--help" or firstArgument == "-?") {
        printUsage();
    } else {
        //no binary mode, therefore using normal mode with old numbering
        systemCode = argv[1];
        unitCode = atoi(argv[2]);
        command = atoi(argv[3]);
    }

    if (wiringPiSetup() == -1) return 1;
    piHiPri(20);
    printf("sending systemCode[%s] unitCode[%i] command[%i]\n", systemCode, unitCode, command);
    RCSwitch mySwitch = RCSwitch();
    mySwitch.setPulseLength(300);
    mySwitch.enableTransmit(PIN);

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
                return -1;
        }
        return 0;
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
                return -1;
        }
        return 0;
    }
}

int multipleSwitch(int argc, char *argv[]) {
    unsigned int PIN = 0;
    bool binaryMode = false;
    char *systemCode;
    unsigned int unitCode;
    unsigned int command;
    unsigned int numberOfActuators;
    unsigned int i;

    printf("multi Mode");

    command = atoi(argv[argc - 1]);

    if (argv[1] = "-b") {
        binaryMode = true;
        numberOfActuators = (argc - 3) / 2;
    } else {
        numberOfActuators = (argc - 2) / 2;
    }

    if (wiringPiSetup() == -1) return 1;
    piHiPri(20);
    printf("sending systemCode[%s] unitCode[%i] command[%i]\n", systemCode, unitCode, command);
    RCSwitch mySwitch = RCSwitch();
    mySwitch.setPulseLength(300);
    mySwitch.enableTransmit(PIN);

    for (i = 1; i < numberOfActuators + 1; i++) {

        int indexSystemCode = 0;
        int indexUnitCode = 0;

        indexSystemCode = 2 + 2 * (i - 1);
        indexUnitCode = 1 + 2 * i;

        systemCode = argv[indexSystemCode];
        unitCode = atoi(argv[indexUnitCode]);

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
//                    return -1;
            }
//            return 0;
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
//                    return -1;
            }
//            return 0;
        }
        return 0;
    }
}
