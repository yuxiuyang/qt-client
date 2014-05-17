/*
 * AnalyseIbp.h
 *
 *  Created on: 9 May, 2014
 *      Author: root
 */

#ifndef ANALYSEIBP_H_
#define ANALYSEIBP_H_
#include "define.h"

class AnalyseIbp {
public:
	AnalyseIbp(void* p);
	virtual ~AnalyseIbp();
	int open_block();
	bool anal_pag(const BYTE* buf,const int len);
	void anal_DataPag(const BYTE* buf,int len);
	void addBuf(const BYTE* buf,int len);
	bool checkData(const BYTE* buf,const int len,const BYTE value);
	void handle(const BYTE* buf,int len);
private:
	void* m_mgr;

	BYTE m_dataBuf[MAX_DATA_BUF];
	int m_curPos;
};

#endif /* AnalyseIbp_H_ */
