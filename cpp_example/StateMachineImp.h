#ifndef _STATE_MACHINE_IMP_H
#define _STATE_MACHINE_IMP_H

#include <thread>

#include "MsgQueueIF.h"
#include "SocketServerIF.h"
#include "StateMachineIF.h"

class StateMachineImp: public StateMachineIF
{
public:
	StateMachineImp(MsgQueueIF* q, SocketServerIF* s): msgQ(q), socketS(s) {};
	virtual ~StateMachineImp();

private:
	virtual void run();
	virtual void stop();
	
	void handleMsg();

	MsgQueueIF* msgQ;
	SocketServerIF* socketS;
	thread* 	runThread;
	bool	stop_flag;
};

#endif
