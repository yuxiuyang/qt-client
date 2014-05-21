/*
 * define.h
 *
 *  Created on: 7 May 2014
 *      Author: root
 */

#ifndef DEFINE_H
#define DEFINE_H
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <iostream>
#include <string.h>
using namespace std;
#define MAX_DATA_BUF 4096
#define ERROR_MSG_BUF 150

typedef unsigned char BYTE;
typedef unsigned long int UNIT;
static BYTE charConvertToHex(char a){
        a = toupper(a);//convert Uppercase letters
        //tolower
        if(a>='0'&&a<='9'){
                return a-'0';
        }
        switch(a){
        case 'A':
                return 10;
        case 'B':
                return 11;
        case 'C':
                return 12;
        case 'D':
                return 13;
        case 'E':
                return 14;
        case 'F':
                return 15;
        default:
                return 100;
        }
        return 100;
};
static BYTE twoBYTEConverToHex(BYTE hight,BYTE low){
        assert(hight<16&&hight>=0);
        assert(low<16&&low>=0);

        //printf("hight<<4 = %d\n",hight<<4);
        return (hight<<4)|low;
}

enum DataSource_{
    PC_Simulator_Link=0x10,//pc 模拟器 连接
    Monitor_UI_Link,//监护仪或UI 连接
    Server_Link
};
enum ClientType_{
    ECG_CLIENT=0,
    SPO2_CLIENT,
    CO2_CLIENT,
    NIBP_CLIENT,
    IBP_CLIENT,
    NARCO_CLIENT,
    CMD_CLIENT,//命令参数
    DISPLAY_CLIENT,
    NONE_CLIENT,
    CLIENT_NUM,
};

struct NotifyMsg_{
    DataSource_ comeForm;
    ClientType_ type;
    bool connectState;
};
struct CmdMsg_{
    DataSource_ comeForm;
    ClientType_ moduleType;
    int         cmd;
    void*      wparam;
    void*      lparam;
};
struct DataMsg_{
    DataSource_ comeForm;
    ClientType_ type;
    BYTE buf[MAX_DATA_BUF];
    int buf_len;
};
enum MsgType_{
    Notify_Msg=0x30,
    Cmd_Msg,
    Data_Msg,
    Client_Error_Msg,
    Server_Error_Msg,
    Link_Msg,
    Link_Error_Msg,
    Link_Request,

};
struct Link{
    int fd;
    DataSource_ comeForm;
    ClientType_ type;
};
struct Msg_{
    MsgType_ type;
    union{
        NotifyMsg_ notifyMsg;
        CmdMsg_    cmdMsg;
        DataMsg_   dataMsg;
        Link       linkMsg;
        char       errorBuf[ERROR_MSG_BUF];
    };
};

enum CONNECT_MSG_TYPE{
    Connect_Error=0x41,
    Connect_Timeout,
    Connect_Success,
    Connect_Failure,
    Connect_Close,
};
/*link msg protocal
  0x99  //head
  0x06  //len
  Link_Msg  //type
  0xxx   //client id
  0xxx   //check
  0xdd   //end


  request link protocal
  0x99  //head
  0x05  //len
  Link_Request  //type
  0xxx   //check
  0xdd   //end

*/
enum LinkState{
    link_not_comfirmId,
    link_and_comfirmId,
    dis_link,
};
enum CMDTYPE{
	NIBP_TYPE,
	MODE_COLLECTDATAS,
	MODE_NORMAL,
};
enum PatientType{
	NIBP_ADULT,					//成人
	NIBP_ENFANT,					//儿童
	NIBP_BABY,						//婴儿
	NIBP_NONE,
};
enum NIBP_STATE{
    NIBP_START=0x87,
    NIBP_STOP,
};

//shutdown save
enum STATE_SAVE{
    NIBP_FRE=1,
    NIBP_READRUM,
    NIBP_TEST,
    NIBP_SHOWDATA,
    NIBP_SENDDATA,//5
    SPO2_FRE,
    SPO2_READRUM,
    SPO2_TEST,
    SPO2_SHOWDATA,
    SPO2_SENDDATA,//10
    ECG_FRE,
    ECG_READRUM,
    ECG_TEST,
    ECG_SHOWDATA,
    ECG_SENDDATA,//15
    IBP_FRE,
    IBP_READRUM,
    IBP_TEST,
    IBP_SHOWDATA,
    IBP_SENDDATA,//20
    CO2_FRE,
    CO2_READRUM,
    CO2_TEST,
    CO2_SHOWDATA,
    CO2_SENDDATA,
    NARCO_FRE,
    NARCO_READRUM,
    NARCO_TEST,
    NARCO_SHOWDATA,
    NARCO_SENDDATA,

    CUR_CLIENT,

    SHUTDOWN_SAVE,

    NIBP_FILE_START_POS,
    NIBP_FILE_END_POS,
    SPO2_FILE_START_POS,
    SPO2_FILE_END_POS,
    ECG_FILE_START_POS,
    ECG_FILE_END_POS,
    IBP_FILE_START_POS,
    IBP_FILE_END_POS,
    CO2_FILE_START_POS,
    CO2_FILE_END_POS,
    NARCO_FILE_START_POS,
    NARCO_FILE_END_POS,
    SHUTDOWN_NOT_SAVE,

    COLLECT_DATA,
    COLLECT_START,

    STATE_COUNT,
};




#endif // DEFINE_H
