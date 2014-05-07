/*
 * nibpmgr.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#include "nibpmgr.h"

NibpMgr::NibpMgr() {
	// TODO Auto-generated constructor stub

}

NibpMgr::~NibpMgr() {
	// TODO Auto-generated destructor stub
}

void NibpMgr::createControl(Fl_Window* ww){
	m_connectBtn = new Fl_Button(20, 20, 80, 30, "connect");
	m_connectBtn->callback(connect,this);
	ww->add(m_connectBtn);

	m_connectBox = new Fl_Box(120,20,100,30,"not connect");
	ww->add(m_connectBox);

	m_disConnectBtn = new Fl_Button(220, 20, 80, 30, "disconnect");
	m_connectBtn->callback(disConnect,this);
	m_disConnectBtn->hide();
	ww->add(m_disConnectBtn);


	m_displayTxt = new Fl_Multiline_Output(20, 100, 470, 200, "");
	ww->add(m_displayTxt);

	{
		Fl_Group* o = new Fl_Group(20, 310, 140, 100);
		o->box(FL_THIN_UP_FRAME);
		{
			Fl_Round_Button* o = new Fl_Round_Button(20, 310, 140, 30,"NIBP_ADULT");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) selectType,this);
			m_patientType = NIBP_ADULT;
		} // Fl_Round_Button* o
		{
			Fl_Round_Button* o = new Fl_Round_Button(20, 340, 140, 30,"NIBP_ENFANT");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) selectType,this);
		} // Fl_Round_Button* o
		{
			Fl_Round_Button* o = new Fl_Round_Button(20, 370, 140, 30,"NIBP_BABY");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) selectType,this);
		} // Fl_Round_Button* o

		o->end();
		ww->add(o);
	}

	m_startNibp = new Fl_Button(240, 310, 80, 30, "start nibp");
	m_startNibp->callback((Fl_Callback*)startNibp);
	ww->add(m_startNibp);
}
void NibpMgr::connect(Fl_Widget *, void *p){
	NibpMgr* pThis = (NibpMgr*)p;
	if(-1 == pThis->m_network.connect()){
		printf("connect failure");
		return;
	}
	pThis->sendIdMsg();
	pThis->m_connectBox->label("connect success");
	pThis->m_connectBtn->hide();
	pThis->m_disConnectBtn->show();
}
void NibpMgr::disConnect(Fl_Widget *, void *p){
	NibpMgr* pThis = (NibpMgr*)p;
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
}
void NibpMgr::startNibp(Fl_Button* b,void* p){
	NibpMgr* pThis = (NibpMgr*)p;
	pThis->sendPatientTypeCmd();
	printf("start nibp\n");
}
void NibpMgr::sendPatientTypeCmd(){
	switch(m_patientType){
	case NIBP_ADULT:
		break;
	case NIBP_ENFANT:
		break;
	case NIBP_BABY:
		break;
	default:
		break;
	}


}
void NibpMgr::sendIdMsg(){
	BYTE tmp[4];
	tmp[0] = 0x99;
	tmp[1] = 0x20;//NIBP_CLIENT
	tmp[2] = tmp[1];
	tmp[3] = 0xdd;

	m_network.send(tmp,4);
}
