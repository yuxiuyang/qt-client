/*
 * nibpmgr.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#include "nibpmgr.h"
#include "mgrdev.h"
#define LINE_LEN 60
static int g_linePos=0;
NibpMgr::NibpMgr() {
	// TODO Auto-generated constructor stub
	m_analNibp = new AnalyseNibp(this);
}

NibpMgr::~NibpMgr() {
	// TODO Auto-generated destructor stub
	delete m_analNibp;
	m_analNibp = NULL;

	disConnect(0,0);
}

void NibpMgr::createControl(Fl_Window* ww){
	m_connectBtn = new Fl_Button(20, 20, 82, 30, "connect");
	m_connectBtn->callback(connect,this);
	ww->add(m_connectBtn);

	m_connectBox = new Fl_Box(120,20,100,30,"not connect");
	ww->add(m_connectBox);

	m_disConnectBtn = new Fl_Button(250, 20, 82, 30, " disconnect");
	m_disConnectBtn->callback(disConnect,this);
	m_disConnectBtn->hide();
	ww->add(m_disConnectBtn);


	m_displayTxt = new Fl_Multiline_Output(20, 100, 470, 200, "");
	m_displayTxt->tooltip("This is an Fl_Multiline_Output widget.");
	ww->add(m_displayTxt);

	{
		Fl_Group* o = new Fl_Group(20, 310, 120, 100);
		o->box(FL_THIN_UP_FRAME);
		{
			Fl_Round_Button* o = new Fl_Round_Button(20, 310, 120, 30,"NIBP_ADULT");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->value(1);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) selectType,this);
			m_patientType = NIBP_ADULT;
		} // Fl_Round_Button* o
		{
			Fl_Round_Button* o = new Fl_Round_Button(20, 340, 120, 30,"NIBP_ENFANT");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) selectType,this);
		} // Fl_Round_Button* o
		{
			Fl_Round_Button* o = new Fl_Round_Button(20, 370, 120, 30,"NIBP_BABY");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) selectType,this);
		} // Fl_Round_Button* o

		o->end();
		ww->add(o);
	}

	m_startNibp = new Fl_Button(240, 310, 80, 30, " start nibp");
	m_startNibp->callback((Fl_Callback*)startNibp,this);
	ww->add(m_startNibp);

	m_clearTxt = new Fl_Button(350, 310, 80, 30, " clear");
	m_clearTxt->callback((Fl_Callback*)clearTxt,this);
	ww->add(m_clearTxt);
}
void NibpMgr::connect(Fl_Widget *, void *p){
	NibpMgr* pThis = (NibpMgr*)p;
	if(-1 == pThis->m_network.connect()){
		printf("connect failure");
		return;
	}
	Fl::add_fd(pThis->m_network.getSockFd(),Data_Arrived_nibp,pThis);
	pThis->sendIdMsg();
	sleep(1);
	pThis->sendPatientTypeCmd();
	pThis->m_connectBox->label("connect success");
	pThis->m_connectBtn->hide();
	pThis->m_disConnectBtn->show();
}
void NibpMgr::disConnect(Fl_Widget *, void *p){
	NibpMgr* pThis = (NibpMgr*)p;
	Fl::remove_fd(pThis->m_network.getSockFd());

	pThis->m_network.disConnect();
	pThis->m_connectBox->label("not connect");
	pThis->m_connectBtn->show();
	pThis->m_disConnectBtn->hide();
}

void NibpMgr::selectType(Fl_Button *b, void *p) {
	NibpMgr* pThis = (NibpMgr*)p;
  char msg[256];
  sprintf(msg, "Label: '%s'\nValue: %d", b->label(),b->value());
  //cb_info->value(msg);
  //cb_info->redraw();
  printf("%s\n",msg);

  if(strcmp(b->label(),"NIBP_ADULT")){
	  pThis->m_patientType = NIBP_ADULT;
  }else if(strcmp(b->label(),"NIBP_ENFANT")){
	  pThis->m_patientType = NIBP_ENFANT;
  }if(strcmp(b->label(),"NIBP_BABY")){
	  pThis->m_patientType = NIBP_BABY;
  }else{
	  pThis->m_patientType = NIBP_NONE;
  }
  pThis->sendPatientTypeCmd();
}
void NibpMgr::startNibp(Fl_Button* b,void* p){
	NibpMgr* pThis = (NibpMgr*)p;
	static int i = 0;
	if(!i){
		pThis->sendPatientTypeCmd();
		MgrDev::getInstance()->sendData(pThis->m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,START_NIBP);
		i ++;
		printf("start nibp\n");
	}else{
		MgrDev::getInstance()->sendData(pThis->m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,STOP_NIBP);
		i = 0;
		printf("start stop\n");
	}


}
void NibpMgr::clearTxt(Fl_Button* b,void* p){
	NibpMgr* pThis = (NibpMgr*)p;
	pThis->m_displayTxt->value("");
	g_linePos = 0;
}
void NibpMgr::sendPatientTypeCmd(){
	switch(m_patientType){
	case NIBP_ADULT:
		MgrDev::getInstance()->sendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_ADULT);
		break;
	case NIBP_ENFANT:
		MgrDev::getInstance()->sendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_ADULT);
		break;
	case NIBP_BABY:
		MgrDev::getInstance()->sendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_BABY);
		break;
	default:
		break;
	}


}
void NibpMgr::sendIdMsg(){
	MgrDev::getInstance()->sendData(m_network.getSockFd(),Link_Msg,NIBP_CLIENT);
}
BYTE receive_buf[1024];
void NibpMgr::Data_Arrived_nibp(int fdt,void *pv){
	memset(receive_buf,0,sizeof(receive_buf));
	int nLen = 0;
	NibpMgr* pthis=(NibpMgr*)pv;
	if(0 != (nLen = read(fdt,(void *)receive_buf,80))){
		/*	send data to analyse	*/
		pthis->m_analNibp->handle(receive_buf,nLen);
	}
}

void NibpMgr::appendData(const BYTE* buf,int len){
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
void NibpMgr::appendData(const char * buf){
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





