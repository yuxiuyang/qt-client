/*
 * simulator_client.h
 *
 *  Created on: 19 May, 2014
 *      Author: root
 */

#ifndef SIMULATOR_CLIENT_H_
#define SIMULATOR_CLIENT_H_

#include "qt_define.h"
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "l_jn.h"
#include "qt_mgrdev.h"
#include <pthread.h>
#include "qt_datamgr.h"

namespace QT_CLIENT{
static CJobNest *g_nibpJob=NULL;//send data task thread.
static CJobNest *g_spo2Job=NULL;//send data task thread.
static CJobNest *g_ecgJob=NULL;//send data task thread.
static CJobNest *g_ibpJob=NULL;//send data task thread.
static CJobNest *g_co2Job=NULL;//send data task thread.
static CJobNest *g_narcoJob=NULL;//send data task thread.

static CJobNest* g_type[CLIENT_NUM];
static bool gModeCollecting;//是否处于 收集数据模式
static qt_DataMgr* mgr[CLIENT_NUM];
static bool bSend[CLIENT_NUM];
static pthread_t g_threadId[CLIENT_NUM];
static bool bInit=false;
void gInitGlobal();
qt_DataMgr* getMgr(ClientType_ id);
bool gConnect(ClientType_ id);
void gDisConnect(ClientType_ id);
void setRecvCallback(ClientType_ id,int (*fun)(const BYTE*,int));

void setModeCollecting(bool val);
bool isCollectingData();

void gSendData(ClientType_ id,BYTE* buf,int len);

void gSendData(int fd,MsgType_ type,ClientType_ id,BYTE* buf,int len);
int  gSendData(int fd,MsgType_ type,ClientType_ clientId);
int  gSendData(int fd,MsgType_ type,ClientType_ clientId,BYTE cmd,BYTE param=0X00);

void* __Invoker(void* arg);

void gSendTestData(int fd,ClientType_ id);
void gStopSendTestData(ClientType_ id);

void sendData_(void* pv);
}
#endif /* SIMULATOR_CLIENT_H_ */
