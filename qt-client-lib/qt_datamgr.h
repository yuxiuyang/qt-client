/*
 * qt_datamgr.h
 *
 *  Created on: 22 May, 2014
 *      Author: root
 */

#ifndef QT_DATAMGR_H_
#define QT_DATAMGR_H_

#include <stdio.h>
#include "qt_network.h"
#include "qt_define.h"
#include "qt_analyse.h"
namespace QT_CLIENT{
class qt_DataMgr {
public:
	qt_DataMgr(ClientType_ clientId);
	virtual ~qt_DataMgr();

	bool connect();
	void disConnect();
	bool isConnect(){
		return m_network.getConnectState();
	}
	void sendTestData();
	void stopSendTestData();
	void sendIdMsg();
	static void recieve(int fdt, void *pv);
	void Data_Arrived_(const BYTE* buf, int len);
	void setCallback(int (*fun)(const BYTE*,int)){
		m_callback_ = fun;
	}

	ClientType_ getCurClientId(){
		return m_clientId;
	}
	int getSockFd(){
		return m_network.getSockFd();
	}
protected:
	ClientType_ m_clientId;
	Network m_network;

	qt_Analyse* m_anal;

	//回调函数
	int (*m_callback_)(const BYTE*,int);
};
}
#endif /* QT_DATAMGR_H_ */
