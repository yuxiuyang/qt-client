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
	Fl_Window *window = new Fl_Window(400,200,500,450);
	//window->position(300,200);

	nibpMgr.createControl(window);

	window->end();
	window->show();
    return Fl::run();
}
