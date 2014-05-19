/*
 * nibpmgr.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#include "spo2mgr.h"
#include "mgrdev.h"
#include "simulator_client.h"
#define LINE_LEN 60
static int g_linePos=0;
Spo2Mgr::Spo2Mgr() {
	// TODO Auto-generated constructor stub
	m_analSpo2 = new AnalyseSpo2(this);
}

Spo2Mgr::~Spo2Mgr() {
	// TODO Auto-generated destructor stub
	delete m_analSpo2;
	m_analSpo2 = NULL;

	disConnect(0,0);
}

void Spo2Mgr::createControl(Fl_Group* ww){
	m_connectBtn = new Fl_Button(20, 30, 82, 30, "connect");
	m_connectBtn->callback(connect,this);
	ww->add(m_connectBtn);

	m_connectBox = new Fl_Box(120,30,100,30,"not connect");
	ww->add(m_connectBox);

	m_disConnectBtn = new Fl_Button(250, 30, 82, 30, " disconnect");
	m_disConnectBtn->callback(disConnect,this);
	m_disConnectBtn->hide();
	ww->add(m_disConnectBtn);


	m_displayTxt = new Fl_Multiline_Output(20, 70, 470, 265, "");
	m_displayTxt->tooltip("This is an Fl_Multiline_Output widget.");
	ww->add(m_displayTxt);

	m_sendTestDataBtn = new Fl_Button(210, 340, 80, 30, " send test data");
	m_sendTestDataBtn->callback((Fl_Callback*)sendTestData,this);
	ww->add(m_sendTestDataBtn);

	m_clearTxt = new Fl_Button(350, 340, 80, 30, " clear");
	m_clearTxt->callback((Fl_Callback*)clearTxt,this);
	ww->add(m_clearTxt);
}
void Spo2Mgr::connect(Fl_Widget *, void *p){
	Spo2Mgr* pThis = (Spo2Mgr*)p;
	if(-1 == pThis->m_network.connect()){
		printf("connect failure");
		return;
	}
	Fl::add_fd(pThis->m_network.getSockFd(),Data_Arrived_spo2,pThis);
	pThis->sendIdMsg();
	//sleep(1);
	pThis->m_connectBox->label("connect success");
	pThis->m_connectBtn->hide();
	pThis->m_disConnectBtn->show();
}
void Spo2Mgr::disConnect(Fl_Widget *, void *p){
	Spo2Mgr* pThis = (Spo2Mgr*)p;
	Fl::remove_fd(pThis->m_network.getSockFd());

	pThis->m_network.disConnect();
	pThis->m_connectBox->label("not connect");
	pThis->m_connectBtn->show();
	pThis->m_disConnectBtn->hide();
}
void Spo2Mgr::sendTestData(Fl_Button* b,void* p){
	printf("spo2mgr send test data start\n");
	Spo2Mgr* pThis = (Spo2Mgr*)p;
	::sendTestData(pThis->m_network.getSockFd());
}
void Spo2Mgr::clearTxt(Fl_Button* b,void* p){
	Spo2Mgr* pThis = (Spo2Mgr*)p;
	pThis->m_displayTxt->value("");
	g_linePos = 0;
}

void Spo2Mgr::sendIdMsg(){
	MgrDev::getInstance()->sendData(m_network.getSockFd(),Link_Msg,SPO2_CLIENT);
}

void Spo2Mgr::Data_Arrived_spo2(int fdt,void *pv){
	BYTE receive_buf[1024];
	memset(receive_buf,0,sizeof(receive_buf));
	int nLen = 0;
	Spo2Mgr* pthis=(Spo2Mgr*)pv;
	if(0 != (nLen = read(fdt,(void *)receive_buf,80))){
		/*	send data to analyse	*/
		pthis->m_analSpo2->handle(receive_buf,nLen);
	}
}

void Spo2Mgr::appendData(const BYTE* buf,int len){
	static string str;
	static char tmp[10];
	str = "";
	for(int i=0;i<len;i++){
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"%02x ",buf[i]);
		str += tmp;
	}
	str += " ";
	appendData(str.c_str());


}
void Spo2Mgr::appendData(const char * buf){
	int len = strlen(buf);
	int pos = m_displayTxt->position();
	m_displayTxt->position(pos+len);

	g_linePos += len;
	if(g_linePos>LINE_LEN){
		m_displayTxt->insert(buf,g_linePos-LINE_LEN);
		m_displayTxt->insert("\n");//换行
		m_displayTxt->insert(buf+g_linePos-LINE_LEN);
		g_linePos = 0;
	}else{
		m_displayTxt->insert(buf);
	}
}




