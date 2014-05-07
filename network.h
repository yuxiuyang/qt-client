#ifndef NETWORK_H
#define NETWORK_H
#include "netdev.h"
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
class Network : public NetDev
{
public:
    Network();

public:
    bool init();
    int connect();
    bool disConnect();
private:
    struct sockaddr_in m_serverAddress;
    unsigned short m_serverPort;
};

#endif // NETWORK_H
