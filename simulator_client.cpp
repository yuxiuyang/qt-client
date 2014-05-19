/*
 * simulator_client.cpp
 *
 *  Created on: 19 May, 2014
 *      Author: root
 */

#include "simulator_client.h"

void stopSendTestData(){
	bSend = false;
}
void sendTestData(int fd){
	bSend = true;
	char testData[] = "123456789abcdefghijklmnopkrstuvwxyz";
	int len = sizeof(testData);
	int pos = 0;

	int count = 6;
	while(bSend){
		if(pos+count<len){
			int num = send(fd,testData+pos,count,0);
			pos += num;
		}else{
			int num = send(fd,testData+pos,pos+count-len,0);
			pos = 0;
			if(num==0)
				continue;
		}
		sleep(1);
	}
}
