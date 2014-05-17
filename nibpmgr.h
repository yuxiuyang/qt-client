/*
 * nibpmgr.h
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#ifndef NIBPMGR_H_
#define NIBPMGR_H_
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
#include "analysenibp.h"

class NibpMgr {
public:
	NibpMgr();
	virtual ~NibpMgr();

	void createControl(Fl_Group* ww);

	static void connect(Fl_Widget *, void *);
	static void disConnect(Fl_Widget *, void *);
	static void selectType(Fl_Button *b, void *);
	static void startNibp(Fl_Button* b,void* p);
	static void clearTxt(Fl_Button* b,void* p);
	void sendIdMsg();
	PatientType getPatientType(){
		return m_patientType;
	}
	void sendPatientTypeCmd();
	static void Data_Arrived_nibp(int fdt,void *pv);
	void appendData(const BYTE* buf,int len);
	void appendData(const char * buf);
private:

	Fl_Button* m_connectBtn;
	Fl_Button* m_disConnectBtn;
	Fl_Box* m_connectBox;
	Fl_Multiline_Output* m_displayTxt;
	Fl_Button* m_startNibp;
	Fl_Button* m_clearTxt;
	Network m_network;

	PatientType m_patientType;

	AnalyseNibp* m_analNibp;
};

#endif /* NIBPMGR_H_ */
