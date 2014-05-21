/*
 * simulator_client.cpp
 *
 *  Created on: 19 May, 2014
 *      Author: root
 */

#include "simulator_client.h"
struct INFO_DATA{
    int fd;
    ClientType_ clientId;
    BYTE buf[1024];
    int len;
    MsgType_ type;
};
void gInitGlobal() {
	if (bInit)
		return;


	for (int i = 0; i < CLIENT_NUM; i++) {
		g_threadId[i] = -1;
		bSend[i] = true;
		g_type[i] = NULL;
		gModeCollecting[i] = false;
	}

	bInit = true;
}

bool getModeCollecting(ClientType_ id){
	if(id>=0 && id<CLIENT_NUM)
		return gModeCollecting[id];

	return false;
}
void setModeCollecting(ClientType_ id,bool val){
	if(id>=0 && id<CLIENT_NUM)
		gModeCollecting[id] = val;
}


void gStopSendTestData(ClientType_ id){
	bSend[id] = false;
}
void gSendTestData(int fd,ClientType_ id){
	if (fd <= 0){
		return;
	}
	if(g_threadId[id]==-1){
		INFO_DATA* info = new INFO_DATA;
		info->fd = fd;
		info->clientId = id;
		info->type = Data_Msg;
		int ret = pthread_create(&g_threadId[id],0, __Invoker,info);
		if (ret != 0) {
	   		perror("pthread_create: failure");
	   		delete info;
	   		g_threadId[id] = -1;
	   		return ;
	   	}
	}


}

void* __Invoker(void* arg){
	INFO_DATA *info = (INFO_DATA*)arg;
	assert(info);

	BYTE testData[256];
	for (int i = 0; i < 256; i++) {
		testData[i] = i;
	}

	bSend[info->clientId] = true;
	int len = sizeof(testData);
	int pos = 0;

	int count = 6;
	while (bSend[info->clientId]) {
		if (pos + count < len) {
			gSendData(info->fd, info->type,info->clientId, testData + pos, count);
			pos += count;
		} else {
			gSendData(info->fd, info->type, info->clientId,testData + pos, pos + count - len);
			pos = 0;
		}
		//sleep(1);
		usleep(1000*500);
	}

	printf("invoker exit,,,,,client=%d\n",info->clientId);
	g_threadId[info->clientId] = -1;
	delete info;

	return NULL;
}
void gSendData(int fd,MsgType_ type,ClientType_ id, BYTE* buf, int len) {
	if (fd <= 0 || id<0 || id>=CLIENT_NUM)
		return;
	//cout<<"sendData fd="<<fd<<endl;
	//驱动任务巢

	if(!g_type[id]){
		g_type[id] = GetJobNest();
		assert(g_type[id]);
	}
	CJobPkg* pkg = NULL;
	pkg = g_type[id]->GetJobPkg(0);

	INFO_DATA* pci = (INFO_DATA*) pkg->Alloc(sizeof(INFO_DATA));
	assert(pci);
	pci->fd = fd;
	pci->len = len;
	pci->type = type;
	pci->clientId = id;
	memcpy(pci->buf, buf, sizeof(BYTE) * len);
//	for(int i=0;i<len;i++){
//		pci->buf[i] = buf[i];
//	}

	pkg->SetExecFunction(sendData_);
	pkg->SetExecParam(pci);
	pkg->SetID(1); //different thread have different source. as to this ID ,can delete the soucre.

	g_type[id]->SubmitJobPkg(pkg);
}
int  gSendData(int fd,MsgType_ type,ClientType_ clientId){
	MgrDev::getInstance()->sendData(fd,type,clientId);
}
int  gSendData(int fd,MsgType_ type,ClientType_ clientId,BYTE cmd,BYTE param){
	MgrDev::getInstance()->sendData(fd,type,clientId,cmd,param);
}





void sendData_(void* pv){
	INFO_DATA* dataMsg = (INFO_DATA*) pv;
	assert(dataMsg);

	int num = MgrDev::getInstance()->sendData(dataMsg->fd, dataMsg->type, dataMsg->clientId,dataMsg->buf, dataMsg->len);
	if(num<=0){
		bSend[dataMsg->clientId] = false;
	}
}
