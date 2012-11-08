#include <wiringPi.h>

char nGroup[6];
int nSwitchNumber;
int nAction;
int nPlugs;
int nTimeout;
int PORT = 11337;

void error(const char *msg);
int getAddr(const char* nGroup, int nSwitchNumber);

PI_THREAD(switchOn);
PI_THREAD(switchOff);
