#ifndef _MSG_HANDLER_IMP
#define _MSG_HANDLER_IMP

#include <queue>
#include <string>
#include <mutex>

#include <condition_variable>
#include "MsgQueueIF.h"

using namespace std;

class MsgQueueImp: public MsgQueueIF
{
public:
	MsgQueueImp(){}
	virtual ~MsgQueueImp();
private:
	virtual void push(int sock, const string& msg);
	virtual MsgType pop(unsigned int timeout);

	queue<MsgType> msg_queue;
	mutex msg_mutex;
	condition_variable msg_cond;
};



#endif
