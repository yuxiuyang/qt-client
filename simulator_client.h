/*
 * simulator_client.h
 *
 *  Created on: 19 May, 2014
 *      Author: root
 */

#ifndef SIMULATOR_CLIENT_H_
#define SIMULATOR_CLIENT_H_

#include "define.h"
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "l_jn.h"
#include "mgrdev.h"
#include <pthread.h>

static CJobNest *g_nibpJob=NULL;//send data task thread.
static CJobNest *g_spo2Job=NULL;//send data task thread.
static CJobNest *g_ecgJob=NULL;//send data task thread.
static CJobNest *g_ibpJob=NULL;//send data task thread.
static CJobNest *g_co2Job=NULL;//send data task thread.
static CJobNest *g_narcoJob=NULL;//send data task thread.

static CJobNest* g_type[CLIENT_NUM];
static bool gModeCollecting[CLIENT_NUM];//是否处于 收集数据模式

static bool bSend[CLIENT_NUM];
static pthread_t g_threadId[CLIENT_NUM];
static bool bInit=false;
void gInitGlobal();
bool getModeCollecting(ClientType_ id);
void setModeCollecting(ClientType_ id,bool val);


void gSendData(int fd,MsgType_ type,ClientType_ id,BYTE* buf,int len);
int  gSendData(int fd,MsgType_ type,ClientType_ clientId);
int  gSendData(int fd,MsgType_ type,ClientType_ clientId,BYTE cmd,BYTE param=0X00);

void* __Invoker(void* arg);

void gSendTestData(int fd,ClientType_ id);
void gStopSendTestData(ClientType_ id);

void sendData_(void* pv);

#endif /* SIMULATOR_CLIENT_H_ */
