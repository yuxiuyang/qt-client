/*
 * qt_nIbpmgr.h
 *
 *  Created on: 22 May, 2014
 *      Author: root
 */

#ifndef QT_NIBPMGR_H_
#define QT_NIBPMGR_H_
#include "qt_datamgr.h"
namespace QT_CLIENT{
class qt_NibpMgr:public qt_DataMgr{
public:
	qt_NibpMgr();
	virtual ~qt_NibpMgr();

	void startNibp(bool bStart);
	void sendPatientTypeCmd();
	void setPatientType(PatientType type);
	PatientType getPatientType(){
		return m_patientType;
	}

private:
	PatientType m_patientType;
};
}
#endif /* QT_NIBPMGR_H_ */
