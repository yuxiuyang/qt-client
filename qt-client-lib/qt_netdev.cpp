/*
 * netdev.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */
#include "qt_netdev.h"
#include <unistd.h>
namespace QT_CLIENT{
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
}
