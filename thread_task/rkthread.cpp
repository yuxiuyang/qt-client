/*****************************************************************************\
	module:	
	file:	rkthread.cpp
	by:		Rick Wang
	date:	2002/03
	description:
		锟斤拷锟斤拷募锟絩kthread.h锟斤拷锟斤拷锟斤拷锟斤拷锟侥硷拷锟斤拷锟斤拷锟斤拷锟斤拷锟绞斤拷锟阶帮拷锟絃inux锟斤拷pthread锟竭筹拷
		锟斤拷牟锟斤拷止锟斤拷埽锟斤拷锟斤拷峁╋拷叱锟斤拷锟绞憋拷锟斤拷锟斤拷锟秸癸拷锟斤拷锟�	-----------------------------------------------------------------------
	modification records:
	1.	by: ?			date: ?
		description: ?
	2.
\*****************************************************************************/
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "rkthread.h"
#include <assert.h>

/*
	锟竭程伙拷锟斤拷锟斤拷
*/
CMutex::CMutex(){
	int iRetVal=pthread_mutex_init(&m_lock,NULL);
	assert(SUCCESS==iRetVal);

	m_InsideThread=NULL;
	m_uLockCount=0;
}

CMutex::~CMutex(){
	int iRetVal;
	iRetVal=pthread_mutex_destroy(&m_lock);
	assert(SUCCESS==iRetVal);
}

int CMutex::Lock(){
	//同一锟斤拷锟竭程诧拷锟斤拷锟节讹拷锟斤拷锟斤拷锟斤拷
	if(m_InsideThread){
		if(pthread_equal(m_InsideThread, pthread_self())){
			m_uLockCount++;
			return SUCCESS;
		}
	}
	int iRetVal=pthread_mutex_lock(&m_lock);
	
	if(SUCCESS==iRetVal){
		m_InsideThread=pthread_self();
		m_uLockCount=1;
	}
	return iRetVal;
}

int CMutex::TryLock(){
	//同一锟斤拷锟竭程诧拷锟斤拷锟节讹拷锟斤拷锟斤拷锟斤拷
	if(m_InsideThread){
		if(pthread_equal(m_InsideThread, pthread_self())){
			m_uLockCount++;
			return SUCCESS;
		}
	}

	int iRetVal=pthread_mutex_trylock(&m_lock);
	if(SUCCESS==iRetVal){
		m_InsideThread=pthread_self();
		m_uLockCount=1;
	}
	return iRetVal;
}

int CMutex::UnLock(){
	if(!m_InsideThread){
		return SUCCESS;
	}
	//直锟斤拷同一锟斤拷锟竭程碉拷锟斤拷锟斤拷锟斤拷眉锟斤拷锟轿�锟斤拷锟斤拷式锟斤拷锟斤拷
	if(pthread_equal(m_InsideThread, pthread_self())){
		m_uLockCount--;
		if(!m_uLockCount){
			pthread_t thTemp=m_InsideThread;
			m_InsideThread=NULL;
			int iRetVal=pthread_mutex_unlock(&m_lock);
			if(SUCCESS!=iRetVal){
				m_InsideThread=thTemp;
				m_uLockCount++;
			}
			return iRetVal;
		}
		else{
			assert(m_uLockCount!=0xffffffff);
			return SUCCESS;
		}
	}
	//锟斤拷锟斤拷锟斤拷俳锟斤拷锟斤拷械锟斤拷叱滩锟斤拷堑锟角帮拷锟斤拷锟斤拷叱蹋锟斤拷蚍祷卮锟斤拷锟�	else{
		return ENOLCK;
}

CDelayer::CDelayer(){
}
CDelayer::~CDelayer(){
}
	
void CDelayer::Delay(ulong ms){
//	struct timespec ts;
//	struct timeval now;
//	int ret=0;
//
//	gettimeofday(&now,NULL);
//	ts.tv_sec=now.tv_sec+(ms/1000);
//	ts.tv_nsec=now.tv_usec*1000+(ms%1000)*1000000;
	
	m_lock.Lock();
//	while(ETIMEDOUT!=ret){
//		printf("ret=%d\n",ret);
//		ret=pthread_cond_timedwait(&m_cond,m_lock.GetMutexPtr(),&ts);
//	}
	usleep(ms);
	m_lock.UnLock();
}


/*
	锟竭筹拷同锟斤拷锟铰硷拷锟斤拷
*/
CEvent::CEvent(){
	int iRetVal=pthread_cond_init(&m_cond,NULL);
	assert(SUCCESS==iRetVal);
	
	m_MutexInsideThread=NULL;
	m_uMutexLockCount=0;
}

CEvent::~CEvent(){
	int iRetVal=pthread_cond_destroy(&m_cond);
	assert(SUCCESS==iRetVal);
}

int CEvent::SetEvent(){
	int iRetVal=pthread_cond_signal(&m_cond);
	return iRetVal;
}

int CEvent::Wait(){
	int iRetVal;
	m_lock.Lock();
	m_MutexInsideThread=m_lock.m_InsideThread;
	m_uMutexLockCount=m_lock.m_uLockCount;
	
   	iRetVal=pthread_cond_wait(&m_cond,m_lock.GetMutexPtr());
   	
	m_lock.m_InsideThread=m_MutexInsideThread;
	m_lock.m_uLockCount=m_uMutexLockCount;
	m_lock.UnLock();
	return iRetVal;
}

int CEvent::TimeWait(long lSec, long lNSec){
	struct timespec t={lSec,lNSec};
	m_lock.Lock();
	m_MutexInsideThread=m_lock.m_InsideThread;
	m_uMutexLockCount=m_lock.m_uLockCount;
	
	int iRetVal=pthread_cond_timedwait(&m_cond,m_lock.GetMutexPtr(), &t);
	
	m_lock.m_InsideThread=m_MutexInsideThread;
	m_lock.m_uLockCount=m_uMutexLockCount;
	m_lock.UnLock();
	return iRetVal;
}

/*锟斤拷露CMutex锟侥接匡拷*/
//锟斤拷锟斤拷
int CEvent::Lock(){
	return m_lock.Lock();
}
//锟斤拷锟斤拷锟斤拷锟斤拷
int CEvent::TryLock(){
	return m_lock.TryLock();
}
//锟斤拷锟斤拷
int CEvent::UnLock(){
	return m_lock.UnLock();
}
