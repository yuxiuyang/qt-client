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
    ECG_CLIENT=0x20,
    SPO2_CLIENT,
    CO2_CLIENT,
    NIBP_CLIENT,
    IBPCO_CLIENT,
    CMD_CLIENT,//命令参数
    DISPLAY_CLIENT,
    NONE_CLIENT,
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

#endif // DEFINE_H
