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
		Fl_Group* o = new Fl_Group(20, 310, 70, 100);
		o->box(FL_THIN_UP_FRAME);
		{
			Fl_Round_Button* o = new Fl_Round_Button(20, 310, 70, 30,"NIBP_AUDAL");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) button_cb);
		} // Fl_Round_Button* o
		{
			Fl_Round_Button* o = new Fl_Round_Button(20, 340, 70, 30,"NIBP_ERTONG");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) button_cb);
		} // Fl_Round_Button* o
		{
			Fl_Round_Button* o = new Fl_Round_Button(20, 370, 70, 30,"NIBP_YINGER");
			o->tooltip("Radio button, only one button is set at a time, in the corresponding group.");
			o->type(102);
			o->down_box(FL_ROUND_DOWN_BOX);
			o->callback((Fl_Callback*) button_cb);
		} // Fl_Round_Button* o

		o->end();
		ww->add(o);
	}
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

void NibpMgr::button_cb(Fl_Button *b, void *) {
  char msg[256];
  sprintf(msg, "Label: '%s'\nValue: %d", b->label(),b->value());
  //cb_info->value(msg);
  //cb_info->redraw();
  printf("%s\n",msg);
}
void NibpMgr::sendIdMsg(){
	BYTE tmp[4];
	tmp[0] = 0x99;
	tmp[1] = 0x20;//NIBP_CLIENT
	tmp[2] = tmp[1];
	tmp[3] = 0xdd;

	m_network.send(tmp,4);
}
