#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>

#include "RCSwitch.h"

#define BUFLEN 512
#define PORT 49880

RCSwitch mySwitch;

void error(const char *s);
