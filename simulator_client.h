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

static CJobNest* g_type[CMD_CLIENT];

static bool bSend[CMD_CLIENT];
static pthread_t g_threadId[CMD_CLIENT];
static bool bInit=false;
void gInitGlobal();
void gSendData(int fd,MsgType_ type,BYTE* buf,int len,ClientType_ id);
int  gSendData(int fd,MsgType_ type,ClientType_ clientId);
int  gSendData(int fd,MsgType_ type,ClientType_ clientId,BYTE cmd);
int  gSendData(int fd,const BYTE* buf,int len);

void* __Invoker(void* arg);

void gSendTestData(int fd,ClientType_ id);
void gStopSendTestData(ClientType_ id);

void sendData_(void* pv);

#endif /* SIMULATOR_CLIENT_H_ */
