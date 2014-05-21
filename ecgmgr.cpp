/*
 * nibpmgr.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#include "ecgmgr.h"
#include "mgrdev.h"
#include "simulator_client.h"
#define LINE_LEN 60
static int g_linePos=0;
EcgMgr::EcgMgr() {
	// TODO Auto-generated constructor stub
	m_analEcg = new AnalyseEcg(this);
}

EcgMgr::~EcgMgr() {
	// TODO Auto-generated destructor stub
	delete m_analEcg;
	m_analEcg = NULL;

	disConnect(0,0);
}

void EcgMgr::createControl(Fl_Group* ww){
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

	{
		Fl_Group* group = new Fl_Group(20, 340, 120, 100);
		group->box(FL_THIN_UP_FRAME);
		{
			Fl_Round_Button* o = new Fl_Round_Button(group->x(), group->y(), 120, 30,"NIBP_ADULT");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->value(1);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) selectType,this);
			m_patientType = NIBP_ADULT;
		} // Fl_Round_Button* o20
		{
			Fl_Round_Button* o = new Fl_Round_Button(group->x(), group->y()+30, 120, 30,"NIBP_ENFANT");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) selectType,this);
		} // Fl_Round_Button* o
		{
			Fl_Round_Button* o = new Fl_Round_Button(group->x(), group->y()+60, 120, 30,"NIBP_BABY");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) selectType,this);
		} // Fl_Round_Button* o

		group->end();
		ww->add(group);
	}

	m_sendTestDataBtn = new Fl_Button(160, 340, 180, 30, " send test data");
	m_sendTestDataBtn->callback((Fl_Callback*)sendTestData,this);
	ww->add(m_sendTestDataBtn);

	m_clearTxt = new Fl_Button(350, 340, 80, 30, " clear");
	m_clearTxt->callback((Fl_Callback*)clearTxt,this);
	ww->add(m_clearTxt);
}
void EcgMgr::connect(Fl_Widget *, void *p){
	EcgMgr* pThis = (EcgMgr*)p;
	if(-1 == pThis->m_network.connect()){
		printf("connect failure");
		return;
	}
	Fl::add_fd(pThis->m_network.getSockFd(),Data_Arrived_ecg,pThis);
	pThis->sendIdMsg();
	//sleep(1);
	pThis->m_connectBox->label("connect success");
	pThis->m_connectBtn->hide();
	pThis->m_disConnectBtn->show();
}
void EcgMgr::disConnect(Fl_Widget *, void *p){
	EcgMgr* pThis = (EcgMgr*)p;
	Fl::remove_fd(pThis->m_network.getSockFd());

	pThis->m_network.disConnect();
	pThis->m_connectBox->label("not connect");
	pThis->m_connectBtn->show();
	pThis->m_disConnectBtn->hide();
}
void EcgMgr::sendTestData(Fl_Button* b,void* p){
	EcgMgr* pThis = (EcgMgr*)p;

	if(!strcmp("stop send test data",pThis->m_sendTestDataBtn->label())){
		printf("EcgMgr stop send test data\n");
		pThis->m_sendTestDataBtn->label("start send test data");
		::gStopSendTestData(ECG_CLIENT);
		return;
	}

	printf("EcgMgr spo2mgr send test data start\n");
	pThis->m_sendTestDataBtn->label("stop send test data");
	::gSendTestData(pThis->m_network.getSockFd(),ECG_CLIENT);
}
void EcgMgr::selectType(Fl_Button *b, void *p) {
	EcgMgr* pThis = (EcgMgr*)p;
  char msg[256];
  sprintf(msg, "Label: '%s'\nValue: %d", b->label(),b->value());
  //cb_info->value(msg);
  //cb_info->redraw();
  printf("%s\n",msg);

  if(!strcmp(b->label(),"NIBP_ADULT")){
	  pThis->m_patientType = NIBP_ADULT;
  }else if(!strcmp(b->label(),"NIBP_ENFANT")){
	  pThis->m_patientType = NIBP_ENFANT;
  }else if(!strcmp(b->label(),"NIBP_BABY")){
	  pThis->m_patientType = NIBP_BABY;
  }else{
	  pThis->m_patientType = NIBP_NONE;
  }
  pThis->sendPatientTypeCmd();
}

void EcgMgr::clearTxt(Fl_Button* b,void* p){
	EcgMgr* pThis = (EcgMgr*)p;
	pThis->m_displayTxt->value("");
	g_linePos = 0;
}
void EcgMgr::sendPatientTypeCmd(){
	printf("sendPatientTypeCmd  m_patientType=%d\n",m_patientType);
	gSendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_STOP);
	switch(m_patientType){
	case NIBP_ADULT:
		gSendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_TYPE,NIBP_ADULT);
		break;
	case NIBP_ENFANT:
		gSendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_TYPE,NIBP_ENFANT);
		break;
	case NIBP_BABY:
		gSendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_TYPE,NIBP_BABY);
		break;
	default:
		break;
	}


}
void EcgMgr::sendIdMsg(){
	gSendData(m_network.getSockFd(),Link_Msg,ECG_CLIENT);
}

void EcgMgr::Data_Arrived_ecg(int fdt,void *pv){
	BYTE receive_buf[1024];
	memset(receive_buf,0,sizeof(receive_buf));
	int nLen = 0;
	EcgMgr* pthis=(EcgMgr*)pv;
	if(0 != (nLen = read(fdt,(void *)receive_buf,80))){
		/*	send data to analyse	*/
		pthis->m_analEcg->handle(receive_buf,nLen);
	}
}

void EcgMgr::appendData(const BYTE* buf,int len){
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
void EcgMgr::appendData(const char * buf){
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





