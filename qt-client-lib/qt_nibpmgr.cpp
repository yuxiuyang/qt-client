/*
 * qt_nIbpmgr.cpp
 *
 *  Created on: 22 May, 2014
 *      Author: root
 */

#include "qt_nibpmgr.h"
#include "simulator_client.h"

namespace QT_CLIENT{
qt_NibpMgr::qt_NibpMgr():qt_DataMgr(NIBP_CLIENT) {
	// TODO Auto-generated constructor stub

}

qt_NibpMgr::~qt_NibpMgr() {
	// TODO Auto-generated destructor stub
}


void qt_NibpMgr::sendPatientTypeCmd(){
	printf("sendPatientTypeCmd  m_patientType=%d\n",m_patientType);
	gSendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_STOP);
	switch(m_patientType){
	case NIBP_ADULT:
		gSendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_TYPE,NIBP_ADULT);
		break;
	case NIBP_ENFANT:
		gSendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_TYPE,NIBP_ENFANT);
		break;
	case NIBP_BABY:
		gSendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_TYPE,NIBP_BABY);
		break;
	default:
		break;
	}

}
void qt_NibpMgr::setPatientType(PatientType type){
	m_patientType = type;
	//sendPatientTypeCmd();
}
void qt_NibpMgr::startNibp(bool bStart){
	if(bStart){
		printf("start nibp\n");
		//sendPatientTypeCmd();
		gSendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_START);

	}else{
		printf("start stop\n");
		gSendData(m_network.getSockFd(),Cmd_Msg,NIBP_CLIENT,NIBP_STOP);
	}
}

}
