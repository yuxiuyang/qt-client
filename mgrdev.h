/*
 * mgrdev.h
 *
 *  Created on: 13 May, 2014
 *      Author: root
 */

#ifndef MGRDEV_H_
#define MGRDEV_H_
#include "define.h"
#include <sys/socket.h>
class MgrDev {
private:
	MgrDev();
public:
	virtual ~MgrDev();

	static MgrDev* getInstance();
	int sendData(int fd,MsgType_ type,ClientType_ clientId,const BYTE* buf,int len);
	int sendData(int fd,MsgType_ type,ClientType_ clientId);
	int sendData(int fd,MsgType_ type,ClientType_ clientId,BYTE cmd);
	int sendData(int fd,const BYTE* buf,int len);
private:
	static MgrDev* m_instance;


};

#endif /* MGRDEV_H_ */
