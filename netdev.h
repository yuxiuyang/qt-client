/*
 * netdev.h
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#ifndef NETDEV_H_
#define NETDEV_H_

class NetDev
{
public:
    NetDev();
    virtual ~NetDev();
    virtual bool init()=0;
    virtual int connect()=0;
    virtual bool disConnect()=0;
public:
    int getSockFd(){
        return m_sockFd;
    }
    bool getConnectState(){
        return m_sockFd==-1?false:true;
    }

protected:
    int m_sockFd;
};

#endif /* NETDEV_H_ */
