/*
 * Usage: ./send <systemCode> <unitCode> <command>
 * Command is 0 for OFF and 1 for ON
 */

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
//#include <iostream>

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
  char* systemCode;
  int unitCode;
  int command;
  std::string firstArgument = argv[1];

  if (firstArgument == "-b") {
    printf("operating in binary mode...\n");
    binaryMode = true;
    //position of data in input is now shifted by 1 because of '-b'-flag
    systemCode = argv[2];
    unitCode = atoi(argv[3]);
    command  = atoi(argv[4]);
  } else {
    //no binary mode, therefore using normal mode with old numbering
    systemCode = argv[1];
    unitCode = atoi(argv[2]);
    command  = atoi(argv[3]);
  }

  if (wiringPiSetup () == -1) return 1;
  piHiPri(20);
  printf("sending systemCode[%s] unitCode[%i] command[%i]\n", systemCode, unitCode, command);
  RCSwitch mySwitch = RCSwitch();
  mySwitch.setPulseLength(300);
  mySwitch.enableTransmit(PIN);

  if (binaryMode) {
    switch (command){
      case 1:
        mySwitch.switchOnBinary(systemCode, unitCode);
        break;
      case 0:
        mySwitch.switchOffBinary(systemCode, unitCode);
        break;
      default:
        printf("command[%i] is unsupported\n", command);
        return -1;
    }
    return 0;
  } else {
    switch(command) {
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
        return -1;
    }
    return 0;
  }
}
