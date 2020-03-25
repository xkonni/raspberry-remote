/*
   RCSwitch - Arduino libary for remote control outlet switches
   Copyright (c) 2011 Suat Özgür.  All right reserved.

Contributors:
- Andre Koehler / info(at)tomate-online(dot)de
- Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
- Skineffect / http://forum.ardumote.com/viewtopic.php?f=2&t=48

Project home: http://code.google.com/p/rc-switch/

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "RCSwitch.h"
#include <iostream>
#include <bitset>

unsigned long RCSwitch::nReceivedValue = NULL;
unsigned int RCSwitch::nReceivedBitlength = 0;
unsigned int RCSwitch::nReceivedDelay = 0;
unsigned int RCSwitch::nReceivedProtocol = 0;
unsigned int RCSwitch::timings[RCSWITCH_MAX_CHANGES];
int RCSwitch::nReceiveTolerance = 60;

RCSwitch::RCSwitch() {
  this->nReceiverInterrupt = -1;
  this->nTransmitterPin = -1;
  RCSwitch::nReceivedValue = NULL;
  this->setPulseLength(350);
  this->setRepeatTransmit(10);
  this->setReceiveTolerance(60);
  this->setProtocol(1);
}

/**
 * Sets the protocol to send.
 */
void RCSwitch::setProtocol(int nProtocol) {
  this->nProtocol = nProtocol;
  if (nProtocol == 1){
    this->setPulseLength(350);
  }
  else if (nProtocol == 2) {
    this->setPulseLength(650);
  }
}

/**
 * Sets the protocol to send with pulse length in microseconds.
 */
void RCSwitch::setProtocol(int nProtocol, int nPulseLength) {
  this->nProtocol = nProtocol;
  if (nProtocol == 1){
    this->setPulseLength(nPulseLength);
  }
  else if (nProtocol == 2) {
    this->setPulseLength(nPulseLength);
  }
}


/**
 * Sets pulse length in microseconds
 */
void RCSwitch::setPulseLength(int nPulseLength) {
  this->nPulseLength = nPulseLength;
}

/**
 * Sets Repeat Transmits
 */
void RCSwitch::setRepeatTransmit(int nRepeatTransmit) {
  this->nRepeatTransmit = nRepeatTransmit;
}

/**
 * Set Receiving Tolerance
 */
void RCSwitch::setReceiveTolerance(int nPercent) {
  RCSwitch::nReceiveTolerance = nPercent;
}


/**
 * Enable transmissions
 *
 * @param nTransmitterPin    Arduino Pin to which the sender is connected to
 */
void RCSwitch::enableTransmit(int nTransmitterPin) {
  this->nTransmitterPin = nTransmitterPin;
  pinMode(this->nTransmitterPin, OUTPUT);
}

/**
 * Disable transmissions
 */
void RCSwitch::disableTransmit() {
  this->nTransmitterPin = -1;
}

/**
 * Switch a remote switch on (Type C Intertechno)
 *
 * @param sFamily  Familycode (a..f)
 * @param nGroup   Number of group (1..4)
 * @param nDevice  Number of device (1..4)
 */
void RCSwitch::switchOn(char sFamily, int nGroup, int nDevice) {
  this->sendTriState( this->getCodeWordC(sFamily, nGroup, nDevice, true) );
}

/**
 * Switch a remote switch off (Type C Intertechno)
 *
 * @param sFamily  Familycode (a..f)
 * @param nGroup   Number of group (1..4)
 * @param nDevice  Number of device (1..4)
 */
void RCSwitch::switchOff(char sFamily, int nGroup, int nDevice) {
  this->sendTriState( this->getCodeWordC(sFamily, nGroup, nDevice, false) );
}

/**
 * Switch a remote switch on (Type B with two rotary/sliding switches)
 *
 * @param nAddressCode  Number of the switch group (1..4)
 * @param nChannelCode  Number of the switch itself (1..4)
 */
void RCSwitch::switchOn(int nAddressCode, int nChannelCode) {
  this->sendTriState( this->getCodeWordB(nAddressCode, nChannelCode, true) );
}

/**
 * Switch a remote switch off (Type B with two rotary/sliding switches)
 *
 * @param nAddressCode  Number of the switch group (1..4)
 * @param nChannelCode  Number of the switch itself (1..4)
 */
void RCSwitch::switchOff(int nAddressCode, int nChannelCode) {
  this->sendTriState( this->getCodeWordB(nAddressCode, nChannelCode, false) );
}

/**
 * Switch a remote switch on (Type A with 10 pole DIP switches)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param nChannelCode  Number of the switch itself (1..4)
 */
void RCSwitch::switchOn(char* sGroup, int nChannel) {
  this->sendTriState( this->getCodeWordA(sGroup, nChannel, true) );
}

/**
 * Switch a remote switch off (Type A with 10 pole DIP switches)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param nChannelCode  Number of the switch itself (1..4)
 */
void RCSwitch::switchOff(char* sGroup, int nChannel) {
  this->sendTriState( this->getCodeWordA(sGroup, nChannel, false) );
}

/**
 * Switch a remote switch on (Type A with 10 pole DIP switches), now with real-binary numbering (see comments in getCodeWordA and getCodeWordD)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param nChannelCode  Number of the switch itself (1..31)
 */
void RCSwitch::switchOnBinary(char* sGroup, int nChannel) {
  this->sendTriState( this->getCodeWordD(sGroup, nChannel, true) );
}

/**
 * Switch a remote switch off (Type A with 10 pole DIP switches), now with real-binary numbering (see comments in getCodeWordA and getCodeWordD)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param nChannelCode  Number of the switch itself (1..31)
 */
void RCSwitch::switchOffBinary(char* sGroup, int nChannel) {
  this->sendTriState( this->getCodeWordD(sGroup, nChannel, false) );
}

/**
 * Returns a char[13], representing the Code Word to be send.
 * A Code Word consists of 9 address bits, 3 data bits and one sync bit but in our case only the first 8 address bits and the last 2 data bits were used.
 * A Code Bit can have 4 different states: "F" (floating), "0" (low), "1" (high), "S" (synchronous bit)
 *
 * +-------------------------------+--------------------------------+-----------------------------------------+-----------------------------------------+----------------------+------------+
 * | 4 bits address (switch group) | 4 bits address (switch number) | 1 bit address (not used, so never mind) | 1 bit address (not used, so never mind) | 2 data bits (on|off) | 1 sync bit |
 * | 1=0FFF 2=F0FF 3=FF0F 4=FFF0   | 1=0FFF 2=F0FF 3=FF0F 4=FFF0    | F                                       | F                                       | on=FF off=F0         | S          |
 * +-------------------------------+--------------------------------+-----------------------------------------+-----------------------------------------+----------------------+------------+
 *
 * @param nAddressCode  Number of the switch group (1..4)
 * @param nChannelCode  Number of the switch itself (1..4)
 * @param bStatus       Wether to switch on (true) or off (false)
 *
 * @return char[13]
 */
char* RCSwitch::getCodeWordB(int nAddressCode, int nChannelCode, boolean bStatus) {
  int nReturnPos = 0;
  static char sReturn[13];

  const char* code[5] = { "FFFF", "0FFF", "F0FF", "FF0F", "FFF0" };
  if (nAddressCode < 1 || nAddressCode > 4 || nChannelCode < 1 || nChannelCode > 4) {
    sReturn[0] = '\0';
    return sReturn;
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
 * Like getCodeWord  (Type A)
 */
char* RCSwitch::getCodeWordA(char* sGroup, int nChannelCode, boolean bStatus) {
  int nReturnPos = 0;
  static char sReturn[13];
  /*
   * The codeword, that needs to be sent, consists of three main parts:
   * char 0 to 4: Group-Number (already binary)
   * char 5 to 9: Socket Number (converted to binary, where the socket number 0-5 sets the only active bit in the return string)
   *              e.g: socket 1 means: bit 1 is on, others off: 10000
   *                   socket 5 means: bit 4 is on, others off: 00010
   * char 10 to 11: Power state, where on means '01' and off means '10'
   */
  const char* code[6] = { "FFFFF", "0FFFF", "F0FFF", "FF0FF", "FFF0F", "FFFF0" };

  if (nChannelCode < 1 || nChannelCode > 5) {
    sReturn[0] = '\0';
    return sReturn;
  }

  for (int i = 0; i<5; i++) {
    if (sGroup[i] == '0') {
      sReturn[nReturnPos++] = 'F';
    } else if (sGroup[i] == '1') {
      sReturn[nReturnPos++] = '0';
    } else {
      sReturn[0] = '\0';
      return sReturn;
    }
  }

  for (int i = 0; i<5; i++) {
    sReturn[nReturnPos++] = code[ nChannelCode ][i];
  }

  if (bStatus) {
    sReturn[nReturnPos++] = '0';
    sReturn[nReturnPos++] = 'F';
  } else {
    sReturn[nReturnPos++] = 'F';
    sReturn[nReturnPos++] = '0';
  }
  sReturn[nReturnPos] = '\0';
  //std::cout << sReturn;
  return sReturn;
}

/**
 * Like getCodeWord  (Type A)
 * Like getCodeWordA, but with real binary socket numbers instead of numbering by position of active bit.
 */

/**
 * To use this function, the sockets need to be numbered in real binary after the following scheme:
 *
 * |no. | old representation | new binary |
 * |--------------------------------------|
 * |   1|              10000 |      00001 |
 * |   2|              01000 |      00010 |
 * |   3|              00100 |      00011 |
 * |   4|              00010 |      00100 |
 * |   5|              00001 |      00101 |
 * |   6|              ----- |      00110 |
 * |   8|              ----- |      01000 |
 * |  16|              ----- |      10000 |
 * |  31|              ----- |      11111 |
 * |--------------------------------------|
 *
 * This means, that now more than 5 sockets can be used per system.
 * It is, indeed, necessary, to set all the sockets used to the new binary numbering system.
 * Therefore, most systems won't work with their dedicated remotes anymor, which
 * only provide buttons for socket A to E, sending only the old representation as noted
 * above.
 */

char* RCSwitch::getCodeWordD(char* sGroup, int nChannelCode, boolean bStatus) {
  int nReturnPos = 0;
  static char sReturn[13];

  /**
   * The codeword, that needs to be sent, consists of three main parts:
   * char 0 to 4: Group-Number (already binary)
   * char 5 to 9: Socket Number (converted to binary, former: the socket number 0-5 sets the only active bit in the return string)
   *              e.g: socket 1 means: bit 1 is on, others off: 10000
   *                   socket 5 means: bit 4 is on, others off: 00010
   *              now: real binary representation of decimal socket number
   * char 10 to 11: Power state, where on means '01' and off means '10'
   */

  //const char* code[6] = { "FFFFF", "0FFFF", "F0FFF", "FF0FF", "FFF0F", "FFFF0" }; //former conversion of socket number to binary

  if (nChannelCode < 1 || nChannelCode > 31) {
    sReturn[0] = '\0';
    return sReturn;
  }

  for (int i = 0; i<5; i++) {
    if (sGroup[i] == '0') {
      sReturn[nReturnPos++] = 'F';
    } else if (sGroup[i] == '1') {
      sReturn[nReturnPos++] = '0';
    } else {
      sReturn[0] = '\0';
      return sReturn;
    }
  }

  std::string str = std::bitset<5>(nChannelCode).to_string(); //to binary;
  std::string temp = str;
  /*if (str.size() == 1) {
    str="0000"+temp;
    } else if (str.size() == 2) {
    str="000"+temp;
    } else if (str.size() == 3) {
    str="00"+temp;
    } else if (str.size() == 4) {
    str = "0"+temp;
    }*/

  for(std::string::size_type i = 0; i < str.size(); ++i) {
    if (str[i] == '0') {
      sReturn[nReturnPos++] = 'F';
    } else if (str[i] == '1') {
      sReturn[nReturnPos++] = '0';
    }
  }

  if (bStatus) {
    sReturn[nReturnPos++] = '0';
    sReturn[nReturnPos++] = 'F';
  } else {
    sReturn[nReturnPos++] = 'F';
    sReturn[nReturnPos++] = '0';
  }
  sReturn[nReturnPos] = '\0';
  //std::cout << sReturn;
  return sReturn;
}

/**
 * Like getCodeWord (Type C = Intertechno)
 */
char* RCSwitch::getCodeWordC(char sFamily, int nGroup, int nDevice, boolean bStatus) {
  static char sReturn[13];
  int nReturnPos = 0;

  if ( (byte)sFamily < 97 || (byte)sFamily > 112 || nGroup < 1 || nGroup > 4 || nDevice < 1 || nDevice > 4) {
    sReturn[0] = '\0';
    return sReturn;
  }

  char* sDeviceGroupCode =  dec2binWzerofill(  (nDevice-1) + (nGroup-1)*4, 4  );
  char familycode[16][5] = { "0000", "F000", "0F00", "FF00", "00F0", "F0F0", "0FF0", "FFF0", "000F", "F00F", "0F0F", "FF0F", "00FF", "F0FF", "0FFF", "FFFF" };
  for (int i = 0; i<4; i++) {
    sReturn[nReturnPos++] = familycode[ (int)sFamily - 97 ][i];
  }
  for (int i = 0; i<4; i++) {
    sReturn[nReturnPos++] = (sDeviceGroupCode[3-i] == '1' ? 'F' : '0');
  }
  sReturn[nReturnPos++] = '0';
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
 * Sends a Code Word
 * @param sCodeWord   /^[10FS]*$/  -> see getCodeWord
 */
void RCSwitch::sendTriState(char* sCodeWord) {
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

void RCSwitch::send(unsigned long Code, unsigned int length) {
  this->send( this->dec2binWzerofill(Code, length) );
}

void RCSwitch::send(char* sCodeWord) {
  for (int nRepeat=0; nRepeat<nRepeatTransmit; nRepeat++) {
    int i = 0;
    while (sCodeWord[i] != '\0') {
      switch(sCodeWord[i]) {
        case '0':
          this->send0();
          break;
        case '1':
          this->send1();
          break;
      }
      i++;
    }
    this->sendSync();
  }
}

void RCSwitch::transmit(int nHighPulses, int nLowPulses) {
  boolean disabled_Receive = false;
  int nReceiverInterrupt_backup = nReceiverInterrupt;
  if (this->nTransmitterPin != -1) {
    if (this->nReceiverInterrupt != -1) {
      this->disableReceive();
      disabled_Receive = true;
    }
    for(int i=0; i<nHighPulses; i++) {
      digitalWrite(this->nTransmitterPin, HIGH);
      delayMicroseconds( this->nPulseLength);
    }
    for(int i=0; i<nLowPulses; i++) {
      digitalWrite(this->nTransmitterPin, LOW);
      delayMicroseconds( this->nPulseLength);
    }
    if(disabled_Receive){
      this->enableReceive(nReceiverInterrupt_backup);
    }
  }
}
/**
 * Sends a "0" Bit
 *                       _
 * Waveform Protocol 1: | |___
 *                       _
 * Waveform Protocol 2: | |__
 */
void RCSwitch::send0() {
  if (this->nProtocol == 1){
    this->transmit(1,3);
  }
  else if (this->nProtocol == 2) {
    this->transmit(1,2);
  }
}

/**
 * Sends a "1" Bit
 *                       ___
 * Waveform Protocol 1: |   |_
 *                       __
 * Waveform Protocol 2: |  |_
 */
void RCSwitch::send1() {
  if (this->nProtocol == 1){
    this->transmit(3,1);
  }
  else if (this->nProtocol == 2) {
    this->transmit(2,1);
  }
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

/**
 * Enable receiving data
 */
void RCSwitch::enableReceive(int interrupt) {
  this->nReceiverInterrupt = interrupt;
  this->enableReceive();
}

void RCSwitch::enableReceive() {
  if (this->nReceiverInterrupt != -1) {
    RCSwitch::nReceivedValue = NULL;
    RCSwitch::nReceivedBitlength = NULL;
  }
}

/**
 * Disable receiving data
 */
void RCSwitch::disableReceive() {
  this->nReceiverInterrupt = -1;
}

bool RCSwitch::available() {
  return RCSwitch::nReceivedValue != NULL;
}

void RCSwitch::resetAvailable() {
  RCSwitch::nReceivedValue = NULL;
}

unsigned long RCSwitch::getReceivedValue() {
  return RCSwitch::nReceivedValue;
}

unsigned int RCSwitch::getReceivedBitlength() {
  return RCSwitch::nReceivedBitlength;
}

unsigned int RCSwitch::getReceivedDelay() {
  return RCSwitch::nReceivedDelay;
}

unsigned int RCSwitch::getReceivedProtocol() {
  return RCSwitch::nReceivedProtocol;
}

unsigned int* RCSwitch::getReceivedRawdata() {
  return RCSwitch::timings;
}

/**
 *
 */
bool RCSwitch::receiveProtocol1(int changeCount){

  unsigned long code = 0;
  unsigned long delay = RCSwitch::timings[0] / 31;
  unsigned long delayTolerance = delay * RCSwitch::nReceiveTolerance * 0.01;

  for (int i = 1; i<changeCount ; i=i+2) {

    if (RCSwitch::timings[i] > delay-delayTolerance && RCSwitch::timings[i] < delay+delayTolerance && RCSwitch::timings[i+1] > delay*3-delayTolerance && RCSwitch::timings[i+1] < delay*3+delayTolerance) {
      code = code << 1;
    } else if (RCSwitch::timings[i] > delay*3-delayTolerance && RCSwitch::timings[i] < delay*3+delayTolerance && RCSwitch::timings[i+1] > delay-delayTolerance && RCSwitch::timings[i+1] < delay+delayTolerance) {
      code+=1;
      code = code << 1;
    } else {
      // Failed
      i = changeCount;
      code = 0;
    }
  }
  code = code >> 1;
  if (changeCount > 6) {    // ignore < 4bit values as there are no devices sending 4bit values => noise
    RCSwitch::nReceivedValue = code;
    RCSwitch::nReceivedBitlength = changeCount / 2;
    RCSwitch::nReceivedDelay = delay;
    RCSwitch::nReceivedProtocol = 1;
  }

  if (code != 0) {
    return true;
  }
  else {
    return false;
  }
}

bool RCSwitch::receiveProtocol2(int changeCount){

  unsigned long code = 0;
  unsigned long delay = RCSwitch::timings[0] / 10;
  unsigned long delayTolerance = delay * RCSwitch::nReceiveTolerance * 0.01;

  for (int i = 1; i<changeCount ; i=i+2) {

    if (RCSwitch::timings[i] > delay-delayTolerance && RCSwitch::timings[i] < delay+delayTolerance && RCSwitch::timings[i+1] > delay*2-delayTolerance && RCSwitch::timings[i+1] < delay*2+delayTolerance) {
      code = code << 1;
    } else if (RCSwitch::timings[i] > delay*2-delayTolerance && RCSwitch::timings[i] < delay*2+delayTolerance && RCSwitch::timings[i+1] > delay-delayTolerance && RCSwitch::timings[i+1] < delay+delayTolerance) {
      code+=1;
      code = code << 1;
    } else {
      // Failed
      i = changeCount;
      code = 0;
    }
  }
  code = code >> 1;
  if (changeCount > 6) {    // ignore < 4bit values as there are no devices sending 4bit values => noise
    RCSwitch::nReceivedValue = code;
    RCSwitch::nReceivedBitlength = changeCount / 2;
    RCSwitch::nReceivedDelay = delay;
    RCSwitch::nReceivedProtocol = 2;
  }

  if (code != 0) {
    return true;
  }
  else {
    return false;
  }
}

/**
 * Turns a decimal value to its binary representation
 */
char* RCSwitch::dec2binWzerofill(unsigned long Dec, unsigned int bitLength){
  static char bin[64];
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    }else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';

  return bin;
}
