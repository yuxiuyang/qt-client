/*
 * qt_datamgr.cpp
 *
 *  Created on: 22 May, 2014
 *      Author: root
 */

#include "qt_datamgr.h"
#include "qt_mgrdev.h"
#include "simulator_client.h"
#include "FL/Fl.H"
namespace QT_CLIENT{
qt_DataMgr::qt_DataMgr(ClientType_ clientId) {
	// TODO Auto-generated constructor stub
	m_clientId = clientId;
	m_anal = new qt_Analyse(this);
	m_callback_ = NULL;
}

qt_DataMgr::~qt_DataMgr() {
	// TODO Auto-generated destructor stub
}
bool qt_DataMgr::connect(){
	if(-1 == m_network.connect()){
		printf("connect failure");
		return false;
	}
	Fl::add_fd(m_network.getSockFd(),recieve,this);
	sendIdMsg();
	return true;
}
void qt_DataMgr::disConnect(){
	Fl::remove_fd(m_network.getSockFd());
	m_network.disConnect();
}
void qt_DataMgr::sendTestData(){
	printf("start send test data\n");
	gSendTestData(m_network.getSockFd(),m_clientId);
}
void qt_DataMgr::stopSendTestData(){
	printf("stop Send Test Data ");
	gStopSendTestData(m_clientId);
}

void qt_DataMgr::sendIdMsg(){
	gSendData(m_network.getSockFd(),Link_Msg,m_clientId);
}

void qt_DataMgr::recieve(int fdt,void *pv){
	BYTE receive_buf[1024];
	memset(receive_buf,0,sizeof(receive_buf));
	int nLen = 0;
	qt_DataMgr* pthis=(qt_DataMgr*)pv;
	if(0 != (nLen = read(fdt,(void *)receive_buf,80))){
		/*	send data to analyse	*/
		pthis->m_anal->handle(receive_buf,nLen);
	}
}

void qt_DataMgr::Data_Arrived_(const BYTE* buf,int len){
	if(m_callback_)
		m_callback_(buf,len);
	/*static string str;
	static char tmp[10];
	str = "";
	for(int i=0;i<len;i++){
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"%02x ",buf[i]);
		str += tmp;
	}
	str += " ";*/
	//appendData(str.c_str());


}

}
