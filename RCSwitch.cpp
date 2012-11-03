#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>
#include "RCSwitch.h"

/**
 * Create new RCSwitch Object
 */
RCSwitch::RCSwitch() {
  if(wiringPiSetupGpio() == -1)
    exit(1);
  this->nReceiverInterrupt = -1;
  this->nRepeatTransmit = 10;
  this->nTransmitterPin = -1;
  this->nProtocol = 1;
  this->setPulseLength(350);
}

/**
 * Sets Repeat Transmits
*/
void RCSwitch::setRepeatTransmit(int nRepeatTransmit) {
  this->nRepeatTransmit = nRepeatTransmit;
}

/**
 * Sets pulse length in microseconds
*/
void RCSwitch::setPulseLength(int nPulseLength) {
  this->nPulseLength = nPulseLength;
}

/**
 * Enable transmissions
 * @param nTransmitterPin    Raspberry GPIO to which the sender is connected to
 */
void RCSwitch::enableTransmit(int nTransmitterPin) {
    this->nTransmitterPin = nTransmitterPin;
    pinMode(this->nTransmitterPin, OUTPUT);
}

/**
 * Like getCodeWord  (Type A)
 */
char* RCSwitch::getCodeWordA(char* sGroup, int nSwitchNumber, bool bStatus) {
  int nReturnPos = 0;
  static char sReturn[13];

  const char* code[6] = { "FFFFF", "0FFFF", "F0FFF", "FF0FF", "FFF0F", "FFFF0" };

  if (strlen(sGroup) != 5 || nSwitchNumber < 1 || nSwitchNumber > 5) {
    return '\0';
  }


  for (int i = 0; i<5; i++) {
    if (sGroup[i] == '0') {
      sReturn[nReturnPos++] = 'F';
    } else {
      sReturn[nReturnPos++] = '0';
    }
  }

  for (int i=0; i<5; i++) {
      sReturn[nReturnPos++] = code[nSwitchNumber][i];
  }

   if (bStatus) {
      sReturn[nReturnPos++] = '0';
      sReturn[nReturnPos++] = 'F';
   } else {
      sReturn[nReturnPos++] = 'F';
      sReturn[nReturnPos++] = '0';
   }

  return sReturn;
}

char* RCSwitch::getCodeWordB(int nAddressCode, int nChannelCode, bool bStatus) {
   int nReturnPos = 0;
   static char sReturn[13];

   const char* code[5] = { "FFFF", "0FFF", "F0FF", "FF0F", "FFF0" };
   if (nAddressCode < 1 || nAddressCode > 4 || nChannelCode < 1 || nChannelCode > 4) {
     printf("0\n");
    return '\0';
   }
   for (int i = 0; i<4; i++) {
     sReturn[nReturnPos++] = code[nAddressCode][i];
   }

   for (int i = 0; i<4; i++) {
     sReturn[nReturnPos++] = code[nChannelCode][i];
   }

   sReturn[nReturnPos++] = 'F';
   sReturn[nReturnPos++] = 'F';
   sReturn[nReturnPos++] = 'F';

   if (bStatus) {
      sReturn[nReturnPos++] = 'F';
   } else {
      sReturn[nReturnPos++] = '0';
   }

   sReturn[nReturnPos] = '\0';

   return sReturn;
}

/**
 * Switch a remote switch on (Type B with two rotary/sliding switches)
 *
 * @param nAddressCode  Number of the switch group (1..4)
 * @param nChannelCode  Number of the switch itself (1..4)
 */
void RCSwitch::switchOn(int nAddressCode, int nChannelCode) {
  printf("switchOn nAddressCode: %d, ChannelCode: %d\n", nAddressCode, nChannelCode);
  this->sendTriState( this->getCodeWordB(nAddressCode, nChannelCode, true) );
}

/**
 * Switch a remote switch off (Type B with two rotary/sliding switches)
 *
 * @param nAddressCode  Number of the switch group (1..4)
 * @param nChannelCode  Number of the switch itself (1..4)
 */
void RCSwitch::switchOff(int nAddressCode, int nChannelCode) {
  printf("switchOff nAddressCode: %d, ChannelCode: %d\n", nAddressCode, nChannelCode);
  this->sendTriState( this->getCodeWordB(nAddressCode, nChannelCode, false) );
}

/**
 * Switch a remote switch on (Type A with 10 pole DIP switches)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param nChannelCode  Number of the switch itself (1..4)
 */
void RCSwitch::switchOn(char* sGroup, int nChannel) {
  printf("switchOn  sGroup: %s channel: %d\n", sGroup, nChannel);
  this->sendTriState( this->getCodeWordA(sGroup, nChannel, true) );
}

/**
 * Switch a remote switch off (Type A with 10 pole DIP switches)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param nChannelCode  Number of the switch itself (1..4)
 */
void RCSwitch::switchOff(char* sGroup, int nChannel) {
  printf("switchOff  sGroup: %s channel: %d\n", sGroup, nChannel);
  this->sendTriState( this->getCodeWordA(sGroup, nChannel, false) );
}

/**
 * Sends a Tri-State "0" Bit
 *            _     _
 * Waveform: | |___| |___
 */
void RCSwitch::sendT0() {
  this->transmit(1,3);
  this->transmit(1,3);
}

/**
 * Sends a Tri-State "1" Bit
 *            ___   ___
 * Waveform: |   |_|   |_
 */
void RCSwitch::sendT1() {
  this->transmit(3,1);
  this->transmit(3,1);
}

/**
 * Sends a Tri-State "F" Bit
 *            _     ___
 * Waveform: | |___|   |_
 */
void RCSwitch::sendTF() {
  this->transmit(1,3);
  this->transmit(3,1);
}

/**
 * Sends a "Sync" Bit
 *                       _
 * Waveform Protocol 1: | |_______________________________
 *                       _
 * Waveform Protocol 2: | |__________
 */
void RCSwitch::sendSync() {
  if (this->nProtocol == 1){
    this->transmit(1,31);
  }
  else if (this->nProtocol == 2) {
    this->transmit(1,10);
  }
}

void RCSwitch::sendTriState(char* sCodeWord) {
  // printf("sendTriState sCodeWord: %s\n", sCodeWord);
  for (int nRepeat=0; nRepeat<nRepeatTransmit; nRepeat++) {
    int i = 0;
    while (sCodeWord[i] != '\0') {
      switch(sCodeWord[i]) {
        case '0':
          this->sendT0();
        break;
        case 'F':
          this->sendTF();
        break;
        case '1':
          this->sendT1();
        break;
      }
      i++;
    }
    this->sendSync();
  }
}

void RCSwitch::transmit(int nHighPulses, int nLowPulses) {
  digitalWrite(this->nTransmitterPin, HIGH);
  delayMicroseconds( this->nPulseLength * nHighPulses);
  digitalWrite(this->nTransmitterPin, LOW);
  delayMicroseconds( this->nPulseLength * nLowPulses);
}

void RCSwitch::delayMicroseconds(int msec) {
  usleep(msec);
}
