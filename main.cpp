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
#include <stdlib.h>
#include <stdio.h>
#include "nibpmgr.h"
bool bKeystroke=false;
void send_msg(int ,int ,int){

}



void connect(Fl_Widget *, void *) {
  printf("\007"); fflush(stdout);
}
void exitSys(Fl_Widget *, void *) {
  exit(0);
}
int main(){
	NibpMgr nibpMgr;
	Fl_Window *window = new Fl_Window(500,580);
	window->position(600,300);

	nibpMgr.createControl(window);
//	Fl_Button *b1 = new Fl_Button(20, 20, 80, 25, "connect");
//	b1->callback(connect,0);
//	//Fl_Label* l1 = new
////    Fl_Button *b2 = new Fl_Button(320,20, 80, 25, "exit");
////    b2->callback(exitSys,0);
//
//    Fl_Multiline_Output* output = new Fl_Multiline_Output(20,100,460,400,"");
//
//    output->insert("hhhh");
//    output->insert("hhhh");


	window->end();
	window->show();
    return Fl::run();
}
