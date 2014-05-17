/*
 * Co2Mgr.h
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#ifndef CO2MGR_H_
#define CO2MGR_H_
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Round_Button.H>
#include <stdlib.h>
#include <stdio.h>
#include "network.h"
#include "define.h"
#include "analyseco2.h"

class Co2Mgr {
public:
	Co2Mgr();
	virtual ~Co2Mgr();

	void createControl(Fl_Group* ww);

	static void connect(Fl_Widget *, void *);
	static void disConnect(Fl_Widget *, void *);
	static void clearTxt(Fl_Button* b,void* p);
	void sendIdMsg();
	static void Data_Arrived_co2(int fdt,void *pv);
	void appendData(const BYTE* buf,int len);
	void appendData(const char * buf);
private:

	Fl_Button* m_connectBtn;
	Fl_Button* m_disConnectBtn;
	Fl_Box* m_connectBox;
	Fl_Multiline_Output* m_displayTxt;
	Fl_Button* m_clearTxt;
	Network m_network;

	AnalyseCo2* m_analCo2;
};

#endif /* Co2Mgr_H_ */
