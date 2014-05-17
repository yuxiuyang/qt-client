/*
 * AnalyseIbp.cpp
 *
 *  Created on: 9 May, 2014
 *      Author: root
 */

#include "analyseibp.h"
#include "ibpmgr.h"
AnalyseIbp::AnalyseIbp(void* p){
	// TODO Auto-generated constructor stub
	m_mgr = p;
	assert(m_mgr);
}

AnalyseIbp::~AnalyseIbp() {
	// TODO Auto-generated destructor stub
}
void AnalyseIbp::handle(const BYTE* buf,int len){
    addBuf(buf,len);
    open_block();
}
int AnalyseIbp::open_block(){
    int ix=0;
    while(ix<m_curPos&&m_dataBuf[ix]!=0x99){//找到 99 开头的包。找不到，则丢掉之前数据
        ix++;
    }
    if(ix!=0){//丢掉之前数据
        memmove(m_dataBuf,m_dataBuf+ix,m_curPos-ix);//这里需要用 m_curPos
        m_curPos=m_curPos-ix;
         //重新初始化
        ix = 0;
     }

     //0x99后的 第二位  即数据包长度
     if(m_curPos<2){
         return 0;//数据长度不够，下次再来解析
     }
     int len = m_dataBuf[1];
     if(m_curPos<len){//未达到完整包长度
          return 0;//数据长度不够，下次再来解析
     }
     //printf("len=%02x-%d\n",len,len);
     if(m_dataBuf[len-1]!=0xdd){//这个包有问题，应丢掉
          // throw a pag
          printf("miss a page\n");
          memcpy(m_dataBuf,m_dataBuf+1,m_curPos-1);//这里需要用 m_curPos
          m_curPos -= 1;
          return 0;
      }

      //收到了一个完整的包，校验
     if(checkData(m_dataBuf+1,len-3,m_dataBuf[len-2])){//check ok
         anal_pag(m_dataBuf,len);
     }

     //analyse finished, delete this page
     memcpy(m_dataBuf,m_dataBuf+len,m_curPos-len);
     m_curPos=m_curPos-len;

     return 0;
}

void AnalyseIbp::addBuf(const BYTE* buf,int len){
    if(m_curPos+len<=MAX_DATA_BUF){
          memmove(m_dataBuf+m_curPos,buf,len);
          m_curPos+=len;
    }
    else{
        if(MAX_DATA_BUF-m_curPos>0){
             memmove(m_dataBuf+m_curPos,buf,MAX_DATA_BUF-m_curPos);
             m_curPos=MAX_DATA_BUF;
        }
    }
}
bool AnalyseIbp::anal_pag(const BYTE* buf,const int len){
    switch(buf[2]){
    case Data_Msg:
        anal_DataPag(buf,len);
        break;
    case Link_Msg:
        //anal_ConnectPag(buf,len);
        break;
    case Link_Request:
    	((IbpMgr*)m_mgr)->sendIdMsg();
    	break;
    default:
        break;
    }

    return true;
}
bool AnalyseIbp::checkData(const BYTE* buf,const int len,const BYTE value){
    BYTE sum=0x00;
    for(int i=0;i<len;i++){
        sum += buf[i];
    }

    return sum==value?true:false;
}
void AnalyseIbp::anal_DataPag(const BYTE* buf,int len){
//	printf("anal_DataPag len=%d\n",len);
//	for(int i=0;i<len;i++){
//		printf("%02x ",buf[i]);
//	}
	//printf("end.....\n");
	((IbpMgr*)m_mgr)->appendData(buf+3,len-5);
}
