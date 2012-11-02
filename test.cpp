/**
 * Testing RCSwitch adaptation for the
 * raspberry pi
 *
 * Setup
 *   Power to pin4
 *   GND   to pin6
 *   DATA  to pin7/gpio4
 */

#include "RCSwitch.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  /**
   * setup
   */
  RCSwitch mySwitch = RCSwitch();
  usleep(50000);
  mySwitch.enableTransmit(4);
  usleep(50000);

  /**
   * blink LED
   */
  //digitalWrite(4, HIGH);
  //usleep(50000);
  //digitalWrite(4, LOW);

  /**
   * Try to switch something on
   */
  //mySwitch.switchOn("00001", 1);
  mySwitch.switchOn("00001", 2);
  //mySwitch.switchOn("00001", 3);
  //mySwitch.switchOn("00001", 4);

  /**
   * wait a moment
   */
  usleep(3000000);

  /**
   * Try to switch something off
   */
  //mySwitch.switchOff("00001", 1);
  mySwitch.switchOff("00001", 2);
  //mySwitch.switchOff("00001", 3);
  //mySwitch.switchOff("00001", 4);
}
