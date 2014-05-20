/*
 * Co2Mgr.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#include "co2mgr.h"
#include "mgrdev.h"
#include "simulator_client.h"
#define LINE_LEN 60
static int g_linePos=0;
Co2Mgr::Co2Mgr() {
	// TODO Auto-generated constructor stub
	m_analCo2 = new AnalyseCo2(this);
}

Co2Mgr::~Co2Mgr() {
	// TODO Auto-generated destructor stub
	delete m_analCo2;
	m_analCo2 = NULL;

	disConnect(0,0);
}

void Co2Mgr::createControl(Fl_Group* ww){
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

	m_sendTestDataBtn = new Fl_Button(160, 340, 180, 30, " send test data");
	m_sendTestDataBtn->callback((Fl_Callback*)sendTestData,this);
	ww->add(m_sendTestDataBtn);

	m_clearTxt = new Fl_Button(350, 340, 80, 30, " clear");
	m_clearTxt->callback((Fl_Callback*)clearTxt,this);
	ww->add(m_clearTxt);
}
void Co2Mgr::connect(Fl_Widget *, void *p){
	Co2Mgr* pThis = (Co2Mgr*)p;
	if(-1 == pThis->m_network.connect()){
		printf("connect failure");
		return;
	}
	Fl::add_fd(pThis->m_network.getSockFd(),Data_Arrived_co2,pThis);
	pThis->sendIdMsg();
	//sleep(1);
	pThis->m_connectBox->label("connect success");
	pThis->m_connectBtn->hide();
	pThis->m_disConnectBtn->show();
}
void Co2Mgr::disConnect(Fl_Widget *, void *p){
	Co2Mgr* pThis = (Co2Mgr*)p;
	Fl::remove_fd(pThis->m_network.getSockFd());

	pThis->m_network.disConnect();
	pThis->m_connectBox->label("not connect");
	pThis->m_connectBtn->show();
	pThis->m_disConnectBtn->hide();
}
void Co2Mgr::sendTestData(Fl_Button* b,void* p){
	Co2Mgr* pThis = (Co2Mgr*)p;

	if(!strcmp("stop send test data",pThis->m_sendTestDataBtn->label())){
		printf("stop send test data\n");
		pThis->m_sendTestDataBtn->label("start send test data");
		::gStopSendTestData(CO2_CLIENT);
		return;
	}

	printf("spo2mgr send test data start\n");
	pThis->m_sendTestDataBtn->label("stop send test data");
	::gSendTestData(pThis->m_network.getSockFd(),CO2_CLIENT);
}
void Co2Mgr::clearTxt(Fl_Button* b,void* p){
	Co2Mgr* pThis = (Co2Mgr*)p;
	pThis->m_displayTxt->value("");
	g_linePos = 0;
}

void Co2Mgr::sendIdMsg(){
	MgrDev::getInstance()->sendData(m_network.getSockFd(),Link_Msg,CO2_CLIENT);
}

void Co2Mgr::Data_Arrived_co2(int fdt,void *pv){
	BYTE receive_buf[1024];
	memset(receive_buf,0,sizeof(receive_buf));
	int nLen = 0;
	Co2Mgr* pthis=(Co2Mgr*)pv;
	if(0 != (nLen = read(fdt,(void *)receive_buf,80))){
		/*	send data to analyse	*/
		pthis->m_analCo2->handle(receive_buf,nLen);
	}
}

void Co2Mgr::appendData(const BYTE* buf,int len){
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
void Co2Mgr::appendData(const char * buf){
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





