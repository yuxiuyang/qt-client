/*
 * mgrdev.cpp
 *
 *  Created on: 13 May, 2014
 *      Author: root
 */

#include "mgrdev.h"
#include <assert.h>
MgrDev* MgrDev::m_instance = new MgrDev();
MgrDev::MgrDev() {
	// TODO Auto-generated constructor stub

}

MgrDev::~MgrDev() {
	// TODO Auto-generated destructor stub
}
MgrDev* MgrDev::getInstance(){
	assert(m_instance);
	return m_instance;
}
int MgrDev::sendData(int fd,MsgType_ type,ClientType_ clientId,const BYTE* buf,int len){
	    assert(len+7<=100);

	    BYTE tmpBuf[100];
	    tmpBuf[0] = 0x99;//start
	    tmpBuf[1] = len+6;//
	    tmpBuf[2] = type;
	    tmpBuf[3] = clientId;


	    BYTE calSum = 0x00;
	    for(int i=0;i<len;i++){
	        tmpBuf[4+i] = buf[i];
	        calSum += buf[i];
	    }
	    tmpBuf[4+len] = tmpBuf[1] + tmpBuf[2] + tmpBuf[3] + calSum;

	    tmpBuf[4+len+1] = 0xdd;//end


	    return sendData(fd,tmpBuf,6+len);
}
int MgrDev::sendData(int fd,MsgType_ type,ClientType_ clientId){
	BYTE tmp[6];
	tmp[0] = 0x99;
	tmp[1] = 0x06;
	tmp[2] = type;
	tmp[3] = clientId;
	tmp[4] = tmp[1]+tmp[2]+tmp[3];
	tmp[5] = 0xdd;
	return sendData(fd,tmp,6);
}

int MgrDev::sendData(int fd,MsgType_ msgType,ClientType_ clientId,BYTE cmd){
	BYTE tmp[7];
	tmp[0] = 0x99;
	tmp[1] = 0x07;
	tmp[2] = msgType;
	tmp[3] = clientId;
	tmp[4] = cmd;
	tmp[5] = tmp[1]+tmp[2]+tmp[3]+tmp[4];
	tmp[6] = 0xdd;
	return sendData(fd,tmp,7);
}
int MgrDev::sendData(int fd,const BYTE* data, int len) {
	printf("sendData start len=%d\n",len);
	for(int i=0;i<len;i++){
		printf("%02x ",data[i]);
	}
	printf("  end....\n");
	int total = 0;
	while (1) {
		int size = send(fd, data + total, len, 0);
		if (size <= 0) {
			printf("maybe a error  send failure,and close fd=%d\n", fd);
			close (fd);
			break;
		}
		total += size;
		if (size < len) {
			len -= size;
			continue;
		} else {
			break;
		}
	}
	return total;
}
