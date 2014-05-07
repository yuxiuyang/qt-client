/*
 * netdev.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#include "netdev.h"
#include <unistd.h>
NetDev::NetDev()
{
    m_sockFd = -1;
}
NetDev::~NetDev()
{
    if(m_sockFd!=-1){
        close(m_sockFd);
    }
}
