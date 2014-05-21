/*
 * analysenibp.h
 *
 *  Created on: 9 May, 2014
 *      Author: root
 */

#ifndef ANALYSEECG_H_
#define ANALYSEECG_H_
#include "define.h"

class AnalyseEcg {
public:
	AnalyseEcg(void* p);
	virtual ~AnalyseEcg();
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

#endif /* ANALYSENIBP_H_ */
