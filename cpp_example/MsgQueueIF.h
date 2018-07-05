#ifndef _MSG_HANDLER_H
#define _MSG_HANDLER_H

#include <string>

using namespace std;

typedef struct MsgStruct
{
	int sock;
	string msg;
} MsgType;

class MsgQueueIF
{
public:
	MsgQueueIF(){}
	virtual ~MsgQueueIF(){}	
	virtual void push(int sock, const string& msg) = 0;
	virtual MsgType pop(unsigned int timeout) = 0;
};


#endif
