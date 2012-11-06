/**
 * RCSwitch daemon for the Raspberry Pi
 *
 * Setup
 *   Power to pin4
 *   GND   to pin6
 *   DATA  to pin7/gpio4
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
 *      echo 00001011 | nc 192.168.1.5 12345
 *
 *    Switch plug 01 on 00001 to off
 *      echo 00001010 | nc 192.168.1.5 12345
 *
 *    Get status of plug 01 on 00001
 *      echo 00001012 | nc 192.168.1.5 12345
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

int main(int argc, char* argv[]) {
  /**
   * setup RCSwitch
   */
  if (wiringPiSetup () == -1) 
    return 1;
  RCSwitch mySwitch = RCSwitch();
  usleep(50000);
  mySwitch.enableTransmit(0);
  usleep(50000);


  nPlugs=10;
  int nState[nPlugs];
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

      /**
       * handle messages
       */
      int nAddr = getAddr(nGroup, nSwitchNumber);
      char msg[13];
      if (nAddr > nPlugs)
        printf("Switch invalid: %s::%d\n", nGroup, nSwitchNumber);
      else {
        switch (nAction) {
          /**
           * off
           */
          case 0:
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
            //sprintf(msg, "nState[%d] = %d", nAddr, nState[nAddr]);
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
  }

  /**
   * terminate
   */
  close(newsockfd);
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

