/*
 * nibpmgr.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#include "nibpmgr.h"
#include <FL/Fl_Window.H>
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


	m_displayTxt = new Fl_Multiline_Output(20,100,460,400,"");
	ww->add(m_displayTxt);
}
void NibpMgr::connect(Fl_Widget *, void *p){
	NibpMgr* pThis = (NibpMgr*)p;
	if(-1 == pThis->m_network.connect()){
		printf("connect failure");
		return;
	}
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
