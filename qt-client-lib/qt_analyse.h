/*
 * qt_analyse.h
 *
 *  Created on: 22 May, 2014
 *      Author: root
 */

#ifndef QT_ANALYSE_H_
#define QT_ANALYSE_H_
#include "qt_define.h"
namespace QT_CLIENT{
class qt_Analyse {
public:
	qt_Analyse(void* p);
	virtual ~qt_Analyse();

	int open_block();
	bool anal_pag(const BYTE* buf,const int len);
	void anal_DataPag(const BYTE* buf,int len);
	void anal_CmdPag(BYTE cmd,BYTE param);
	void addBuf(const BYTE* buf,int len);
	bool checkData(const BYTE* buf,const int len,const BYTE value);
	void handle(const BYTE* buf,int len);
private:
	void* m_mgr;

	BYTE m_dataBuf[MAX_DATA_BUF];
	int m_curPos;
};
}//end namespace
#endif /* QT_ANALYSE_H_ */
