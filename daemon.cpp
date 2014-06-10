/**
 * RCSwitch daemon for the Raspberry Pi
 *
 * Setup
 *   Power to pin4
 *   GND   to pin6
 *   DATA  to pin11/gpio0
 *
 * Usage
 *   send xxxxxyyz to ip:port
 *   xxxxx  encoding
 *          00001 for first channel
 *   yy     plug
 *          01 for plug A
 *   z      action
 *          0:off|1:on|2:status
 *
 *  Examples of remote actions
 *    Switch plug 01 on 00001 to on
 *      echo 01111011 | nc localhost 11337
 *
 *    Switch plug 01 on 00001 to off
 *      echo 01111010 | nc localhost 11337
 *
 *    Get status of plug 01 on 00001
 *      echo 01111012 | nc localhost 11337
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "daemon.h"
#include "RCSwitch.h"

RCSwitch mySwitch;

int main(int argc, char* argv[]) {
  /**
   * Setup wiringPi and RCSwitch
   * set high priority scheduling
   */
  if (wiringPiSetup () == -1)
    return 1;
  piHiPri(20);
  mySwitch = RCSwitch();
  mySwitch.setPulseLength(300);
  usleep(50000);
  mySwitch.enableTransmit(0);

  nPlugs=153;
  int nState[nPlugs];
  nTimeout=0;
  memset(nState, 0, sizeof(nState));

  /**
   * setup socket
   */
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = PORT;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  /*
   * start listening
   */
  while (true) {
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                (struct sockaddr *) &cli_addr,
                &clilen);
    if (newsockfd < 0)
      error("ERROR on accept");
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0)
      error("ERROR reading from socket");


    printf("message: %s\n", buffer);
    if (strlen(buffer) >= 8) {
      for (int i=0; i<5; i++) {
        nGroup[i] = buffer[i];
      }
      nGroup[5] = '\0';

      for (int i=5;i<7; i++) {
        nSwitchNumber = buffer[i]-48;
      }
      nAction = buffer[7]-48;
      nTimeout=0;

printf("nGroup: %s\n", nGroup);
printf("nSwitchNumber: %i\n", nSwitchNumber);
printf("nAction: %i\n", nAction);

      if (strlen(buffer) >= 9) nTimeout = buffer[8]-48;
      if (strlen(buffer) >= 10) nTimeout = nTimeout*10+buffer[9]-48;
      if (strlen(buffer) >= 11) nTimeout = nTimeout*10+buffer[10]-48;

      /**
       * handle messages
       */
      int nAddr = getAddr(nGroup, nSwitchNumber);
printf("nAddr: %i\n", nAddr);
printf("nPlugs: %i\n", nPlugs);
      char msg[13];
      if (nAddr > nPlugs) {
        printf("Switch out of range: %s:%d\n", nGroup, nSwitchNumber);
        n = write(newsockfd,"2",1);
      }
      else {
        switch (nAction) {
          /**
           * off
           */
          case 0:
            //piThreadCreate(switchOff);
            mySwitch.switchOff(nGroup, nSwitchNumber);
            nState[nAddr] = 0;
            //sprintf(msg, "nState[%d] = %d", nAddr, nState[nAddr]);
            sprintf(msg, "%d", nState[nAddr]);
            n = write(newsockfd,msg,1);
            break;
          /**
           * on
           */
          case 1:
            //piThreadCreate(switchOn);
            mySwitch.switchOn(nGroup, nSwitchNumber);
            nState[nAddr] = 1;
            //sprintf(msg, "nState[%d] = %d", nAddr, nState[nAddr]);
            sprintf(msg, "%d", nState[nAddr]);
            n = write(newsockfd,msg,1);
            break;
          /**
           * status
           */
          case 2:
            sprintf(msg, "nState[%d] = %d", nAddr, nState[nAddr]);
            sprintf(msg, "%d", nState[nAddr]);
            n = write(newsockfd,msg,1);
            break;
        }
      }
    }
    else {
      printf("message corrupted or incomplete");
    }


    if (n < 0)
      error("ERROR writing to socket");

    close(newsockfd);
  }

  /**
   * terminate
   */
  //close(newsockfd);
  close(sockfd);
  return 0;
}

/**
 * error output
 */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/**
 * calculate the array address of the power state
 */
int getAddr(const char* nGroup, int nSwitchNumber) {
  int bin = atoi(nGroup);
  int base = 1;
  int dec = 0;
  int digit;

  while(bin!=0)
  {
       digit=bin%10;
       dec=dec+digit*base;
       bin=bin/10;
       base=base*2;
    }

  return (dec-1)*5+nSwitchNumber-1;
}

PI_THREAD(switchOn) {
  printf("switchOnThread: %d\n", nTimeout);
  char tGroup[6];
  int tSwitchNumber;
  memcpy(tGroup, nGroup, sizeof(tGroup));
  tSwitchNumber = nSwitchNumber;
  sleep(nTimeout*60);
  mySwitch.switchOn(tGroup, tSwitchNumber);
  return 0;
}

PI_THREAD(switchOff) {
  printf("switchOffThread: %d\n", nTimeout);
  char tGroup[6];
  int tSwitchNumber;
  memcpy(tGroup, nGroup, sizeof(tGroup));
  tSwitchNumber = nSwitchNumber;
  sleep(nTimeout*60);
  mySwitch.switchOff(tGroup, tSwitchNumber);
  return 0;
}

