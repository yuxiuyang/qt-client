/*
 * nibpmgr.h
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#ifndef IBPMGR_H_
#define IBPMGR_H_
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
#include "analyseibp.h"

class IbpMgr {
public:
	IbpMgr();
	virtual ~IbpMgr();

	void createControl(Fl_Group* ww);

	static void connect_click(Fl_Widget *, void *);
	static void disConnect_click(Fl_Widget *, void *);
	static void clearTxt_click(Fl_Button* b,void* p);
	static void sendTestData_click(Fl_Button* b,void* p);
	void sendTestData();
	void stopSendTestData();
	void sendIdMsg();
	static void Data_Arrived_ibp(int fdt,void *pv);
	void appendData(const BYTE* buf,int len);
	void appendData(const char * buf);
private:

	Fl_Button* m_connectBtn;
	Fl_Button* m_disConnectBtn;
	Fl_Box* m_connectBox;
	Fl_Button* m_sendTestDataBtn;
	Fl_Multiline_Output* m_displayTxt;
	Fl_Button* m_clearTxt;
	Network m_network;

	AnalyseIbp* m_analIbp;
};

#endif /* NIBPMGR_H_ */
