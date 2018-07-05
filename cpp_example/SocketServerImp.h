#ifndef _SOCKET_S_H
#define _SOCKET_S_H

#include <string>
#include <map>
#include <iostream>
#include<poll.h>

#include "SocketServerIF.h"

#define MAX_CLIENT 1024


using namespace std;

class SocketServerImp: public SocketServerIF
{
public:
	SocketServerImp();
	~SocketServerImp(){}

private:
	int setup(MsgQueueIF* q, const string& ip, int port);
	int sendMsg(int sock,const string& msg);
	int readMsg(int sock);
	void closeSocket(int sock);
	void run();

	struct pollfd peerfd[MAX_CLIENT];
	map<int, string> buf_map; //for unfinished msg indexed by socket id		
	MsgQueueIF* msgQ = NULL;	
	const string MSG_END = "]]>]]>\r\n";
	int nfds;
};

#endif
