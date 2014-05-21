/*
 * analysenibp.cpp
 *
 *  Created on: 9 May, 2014
 *      Author: root
 */

#include "analysenibp.h"
#include "nibpmgr.h"
#include "simulator_client.h"
AnalyseNibp::AnalyseNibp(void* p){
	// TODO Auto-generated constructor stub
	m_mgr = p;
	assert(m_mgr);
}

AnalyseNibp::~AnalyseNibp() {
	// TODO Auto-generated destructor stub
}
void AnalyseNibp::handle(const BYTE* buf,int len){
    addBuf(buf,len);
    open_block();
}
int AnalyseNibp::open_block(){
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

void AnalyseNibp::addBuf(const BYTE* buf,int len){
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
bool AnalyseNibp::anal_pag(const BYTE* buf,const int len){
	printf("nibp anal_pag   .......\n");
	for(int i=0;i<len;i++){
		printf("%02x ",buf[i]);
	}
	printf(".......end.....\n");
    switch(buf[2]){
    case Data_Msg:
        anal_DataPag(buf,len);
        break;
    case Link_Msg:
        //anal_ConnectPag(buf,len);
        break;
    case Cmd_Msg:
    	anal_CmdPag(buf[4],buf[5]);
    	break;
    case Link_Request:
    	((NibpMgr*)m_mgr)->sendIdMsg();
    	break;
    default:
        break;
    }

    return true;
}
bool AnalyseNibp::checkData(const BYTE* buf,const int len,const BYTE value){
    BYTE sum=0x00;
    for(int i=0;i<len;i++){
        sum += buf[i];
    }

    return sum==value?true:false;
}
void AnalyseNibp::anal_DataPag(const BYTE* buf,int len){
	((NibpMgr*)m_mgr)->appendData(buf+3,len-5);
}
void AnalyseNibp::anal_CmdPag(BYTE cmd,BYTE param){
	printf("AnalyseNibp  anal_CmdPag  cmd=%02x\n",cmd);
	switch(cmd){
	case MODE_COLLECTDATAS:
		setModeCollecting(NIBP_CLIENT,true);
		((NibpMgr*)m_mgr)->sendTestData();
		break;
	case MODE_NORMAL:
		setModeCollecting(NIBP_CLIENT,false);
		((NibpMgr*)m_mgr)->stopSendTestData();
		break;
	}
}
