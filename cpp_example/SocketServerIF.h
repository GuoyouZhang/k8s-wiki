#ifndef _SOCKET_S_IF_H
#define _SOCKET_S_IF_H

#include <string>
#include "MsgQueueIF.h"

#define MAX_CLIENT 1024

using namespace std;

class SocketServerIF
{
public:
	virtual ~SocketServerIF(){}
	virtual int setup(MsgQueueIF* q, const string& ip, int port) = 0;
	virtual int sendMsg(int sock,const string& msg) = 0;
	virtual void closeSocket(int sock) = 0;
	virtual void run() = 0;
};

#endif
