/*
 * main.cpp
 *
 *  Created on: 7 May, 2014
 *      Author: root
 */

#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Clock.H>
#include <FL/Fl_Wizard.H>
#include <stdlib.h>
#include <FL/Fl_Return_Button.H>
#include <stdlib.h>
#include <stdio.h>
#include "nibpmgr.h"
#include "spo2mgr.h"
#include "ecgmgr.h"
#include "co2mgr.h"
#include "ibpmgr.h"
#include "narcomgr.h"
bool bKeystroke=false;
void send_msg(int ,int ,int){

}





void connect(Fl_Widget *, void *) {
  printf("\007"); fflush(stdout);
}
void exitSys(Fl_Widget *, void *) {
  exit(0);
}

static void cb_3(Fl_Button*, void*) {

printf("start button cb_3\n");
}
int main(){
	NibpMgr nibpMgr;
	Spo2Mgr spo2Mgr;
	EcgMgr ecgMgr;
	IbpMgr ibpMgr;
	Co2Mgr co2Mgr;
	NarcoMgr narcoMgr;
	//Fl_Window *window = new Fl_Window(240,100,500,450,"client");
	//window->position(300,200);

	//nibpMgr.createControl(window);

	Fl_Double_Window *foo_window = new Fl_Double_Window(240,100,500,450,"client");
	{
//		Fl_Box* o = new Fl_Box(95, 0, 130, 35, "class Fl_Tabs");
//		o->labeltype(FL_ENGRAVED_LABEL);
//		o->labelfont(1);
	} // Fl_Box* o
	{
		Fl_Tabs* o = new Fl_Tabs(2, 2, foo_window->w()-4, foo_window->h()-4);
		//o->color((Fl_Color)8);
		o->tooltip("the various index cards test different aspects of the Fl_Tabs widget");
		o->selection_color((Fl_Color) 4);
		o->labelcolor(FL_BACKGROUND2_COLOR);
		{
			Fl_Group* o = new Fl_Group(2, 20, foo_window->w()-4, foo_window->h()-4, "NIBP");
			o->tooltip("this Tab tests correct keyboard navigation between text input fields");
			o->selection_color((Fl_Color) 2);
			nibpMgr.createControl(o);
			o->end();
			Fl_Group::current()->resizable(o);
		} // Fl_Group* o
		{
			Fl_Group* o = new Fl_Group(2, 20, foo_window->w()-4, foo_window->h()-4, "SPO2");
			o->tooltip("tab2 tests among other things the cooperation of modal windows and tabs");
			o->selection_color((Fl_Color) 2);
			spo2Mgr.createControl(o);
			o->hide();
			o->end();
			//Fl_Group::current()->resizable(o);
		} // Fl_Group* o
		{
			Fl_Group* o = new Fl_Group(2, 20, foo_window->w()-4, foo_window->h()-4, "ECG");
			o->tooltip("tab2 tests among other things the cooperation of modal windows and tabs");
			o->selection_color((Fl_Color) 2);
			ecgMgr.createControl(o);
			o->hide();
			o->end();
		} // Fl_Group* o
		{
			Fl_Group* o = new Fl_Group(2, 20, foo_window->w()-4, foo_window->h()-4, "IBP");
			o->tooltip("tab2 tests among other things the cooperation of modal windows and tabs");
			o->selection_color((Fl_Color) 2);
			ibpMgr.createControl(o);
			o->hide();
			o->end();
		} // Fl_Group* o
		{
			Fl_Group* o = new Fl_Group(2, 20, foo_window->w()-4, foo_window->h()-4, "CO2");
			o->tooltip("tab2 tests among other things the cooperation of modal windows and tabs");
			o->selection_color((Fl_Color) 2);
			co2Mgr.createControl(o);
			o->hide();
			o->end();
		} // Fl_Group* o
		{
			Fl_Group* o = new Fl_Group(2, 20, foo_window->w()-4, foo_window->h()-4, "NARCO");
			o->tooltip("tab2 tests among other things the cooperation of modal windows and tabs");
			o->selection_color((Fl_Color) 2);
			narcoMgr.createControl(o);
			o->hide();
			o->end();
		} // Fl_Group* o
	}

	foo_window->end();
	foo_window->show();
    return Fl::run();
}
