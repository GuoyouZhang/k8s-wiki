#include<sys/socket.h>
#include<unistd.h>
#include <iostream>
#include "MsgQueueImp.h"


MsgQueueImp::~MsgQueueImp()
{
}

void MsgQueueImp::push(int sock, const string& msg)
{
	unique_lock<std::mutex> lock(msg_mutex);
	MsgType m;
	m.sock = sock;
	m.msg = msg;

	msg_queue.push(m);
	msg_cond.notify_one();
}

MsgType MsgQueueImp::pop(unsigned int timeout)
{
	unique_lock<std::mutex> lock(msg_mutex);
	if(msg_queue.empty())
	{
		msg_cond.wait_for(lock, chrono::seconds(timeout));
	}
	if(!msg_queue.empty())
	{
		MsgType m = msg_queue.front();
		msg_queue.pop();
		return m;
	}
	else
	{
		MsgType m;
		m.sock = -1;
		return m;
	}
}

