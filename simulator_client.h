/*
 * simulator_client.h
 *
 *  Created on: 19 May, 2014
 *      Author: root
 */

#ifndef SIMULATOR_CLIENT_H_
#define SIMULATOR_CLIENT_H_

#include "define.h"
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
static bool bSend;
void sendTestData(int fd);
void stopSendTestData();


#endif /* SIMULATOR_CLIENT_H_ */
