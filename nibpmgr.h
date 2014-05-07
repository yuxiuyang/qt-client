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
#include <stdlib.h>
#include <stdio.h>
#include "network.h"
class NibpMgr {
public:
	NibpMgr();
	virtual ~NibpMgr();

	void createControl(Fl_Window* ww);

	static void connect(Fl_Widget *, void *);
	static void disConnect(Fl_Widget *, void *);
private:

	Fl_Button* m_connectBtn;
	Fl_Button* m_disConnectBtn;
	Fl_Box* m_connectBox;
	Fl_Multiline_Output* m_displayTxt;

	Network m_network;
};

#endif /* NIBPMGR_H_ */
