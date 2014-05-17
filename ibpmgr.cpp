/*
 * nibpmgr.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#include "ibpmgr.h"
#include "mgrdev.h"
#define LINE_LEN 60
static int g_linePos=0;
IbpMgr::IbpMgr() {
	// TODO Auto-generated constructor stub
	m_analIbp = new AnalyseIbp(this);
}

IbpMgr::~IbpMgr() {
	// TODO Auto-generated destructor stub
	delete m_analIbp;
	m_analIbp = NULL;

	disConnect(0,0);
}

void IbpMgr::createControl(Fl_Group* ww){
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

	m_clearTxt = new Fl_Button(350, 340, 80, 30, " clear");
	m_clearTxt->callback((Fl_Callback*)clearTxt,this);
	ww->add(m_clearTxt);
}
void IbpMgr::connect(Fl_Widget *, void *p){
	IbpMgr* pThis = (IbpMgr*)p;
	if(-1 == pThis->m_network.connect()){
		printf("connect failure");
		return;
	}
	Fl::add_fd(pThis->m_network.getSockFd(),Data_Arrived_ibp,pThis);
	pThis->sendIdMsg();
	//sleep(1);
	pThis->m_connectBox->label("connect success");
	pThis->m_connectBtn->hide();
	pThis->m_disConnectBtn->show();
}
void IbpMgr::disConnect(Fl_Widget *, void *p){
	IbpMgr* pThis = (IbpMgr*)p;
	Fl::remove_fd(pThis->m_network.getSockFd());

	pThis->m_network.disConnect();
	pThis->m_connectBox->label("not connect");
	pThis->m_connectBtn->show();
	pThis->m_disConnectBtn->hide();
}

void IbpMgr::clearTxt(Fl_Button* b,void* p){
	IbpMgr* pThis = (IbpMgr*)p;
	pThis->m_displayTxt->value("");
	g_linePos = 0;
}

void IbpMgr::sendIdMsg(){
	MgrDev::getInstance()->sendData(m_network.getSockFd(),Link_Msg,IBP_CLIENT);
}

void IbpMgr::Data_Arrived_ibp(int fdt,void *pv){
	BYTE receive_buf[1024];
	memset(receive_buf,0,sizeof(receive_buf));
	int nLen = 0;
	IbpMgr* pthis=(IbpMgr*)pv;
	if(0 != (nLen = read(fdt,(void *)receive_buf,80))){
		/*	send data to analyse	*/
		pthis->m_analIbp->handle(receive_buf,nLen);
	}
}

void IbpMgr::appendData(const BYTE* buf,int len){
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
void IbpMgr::appendData(const char * buf){
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





