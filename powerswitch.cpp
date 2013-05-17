/*
 * Receive commands from the PowerSwitch Android App
 * http://power-switch.eu/download/
 * This app is designed to work on the ConnAir
 *
 * And execute them on the Raspberry Pi
 *
*/
#include "powerswitch.h"

int main(void) {
  if (wiringPiSetup() == -1)
    return 1;
  piHiPri(20);
  mySwitch = RCSwitch();
  mySwitch.setPulseLength(300);
  usleep(50000);
  mySwitch.enableTransmit(0);

  struct sockaddr_in si_server, si_client;

  int s, recv_len;
  unsigned int slen = sizeof(si_client);
  char buf[BUFLEN];

  // create a UDP socket
  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    error("socket");
  }

  // initialize with 0
  memset((char *) &si_server, 0, sizeof(si_server));

  si_server.sin_family = AF_INET;
  si_server.sin_port = htons(PORT);
  si_server.sin_addr.s_addr = htonl(INADDR_ANY);

  // bind socket to port
  if( bind(s, (struct sockaddr*)&si_server, sizeof(si_server) ) == -1) {
    error("bind");
  }

  // keep listening for data
  while(1) {
    fflush(stdout);

    // read datagram from server socket
    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_client, &slen)) == -1) {
      error("recvfrom()");
    }

#ifdef DEBUG
    // print details of the client/peer and the data received
    printf("Received packet from %s:%d\n",
      inet_ntoa(si_client.sin_addr),
      ntohs(si_client.sin_port));
    printf("Data: %s\n" , buf);
#endif

    std::string str = buf;
    if (std::string::npos != str.find("TXP")) {
      str.replace(0, 4, "");
      std::vector<int> vect;
      std::stringstream ss(str);

      // transform stringstream into vector
      int j;
      while (ss >> j) {
        vect.push_back(j);

       if (ss.peek() == ',')
         ss.ignore();
      }

      int l,i;
      // flip sequence 22
      l = 22;
      i = 6 + 2*(l-1);
      if(vect[i] == 3) { vect[i] = 1;
      } else { vect[i] = 3; }
      if(vect[i+1] == 3) { vect[i+1] = 1;
      } else { vect[i+1] = 3; }

      for(int count=0; count < vect[2]; count++) {
        for(int ind=6; ind < (int)vect.size()-1; ind+=2) {
          mySwitch.transmit(vect[ind], vect[ind+1]);
        }
      }
    }
  }

  close(s);
  return 0;
}

void error(const char *s) {
  perror(s);
  exit(1);
}
