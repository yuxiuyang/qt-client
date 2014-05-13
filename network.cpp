/*
 * network.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#include "network.h"
#include <signal.h>
#include <errno.h>
Network::Network()
{
    memset(&m_serverAddress,0,sizeof(m_serverAddress));
    m_serverPort = 8090;
}
bool Network::init(){
    m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == m_sockFd)
    {
        printf("sockef fail\n");
        return false;
    }
    bzero(&m_serverAddress,sizeof(struct sockaddr_in));
    m_serverAddress.sin_family=AF_INET;
    m_serverAddress.sin_addr.s_addr= inet_addr("127.0.0.1");
    m_serverAddress.sin_port=htons(m_serverPort);
    //printf("s_addr = %#x ,port : %#x\r\n",m_serverAddress.sin_addr.s_addr,m_serverAddress.sin_port);

    return true;
}

int Network::connect(){
	if(m_sockFd!=-1) close(m_sockFd);
    init();
    printf("connect m_sockFd=%d\n",m_sockFd);

    if(-1 == ::connect(m_sockFd,(struct sockaddr *)(&m_serverAddress), sizeof(struct sockaddr)))
    {
        //printf("connect fail !\r\n");
        printf("connect fail errno=%d\n",errno);
        m_sockFd = -1;
        return -1;
    }
    printf("connect ok !\r\n");

    //忽略SIGPIPE 信号
    signal(SIGPIPE,SIG_IGN);
    return m_sockFd;
}
bool Network::disConnect(){
    close(m_sockFd);
    printf("close socket fd=%d\n",m_sockFd);
    m_sockFd = -1;
    return true;
}
