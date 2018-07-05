#include<sys/socket.h>
#include<unistd.h>
#include <iostream>
#include "StateMachineImp.h"


StateMachineImp::~StateMachineImp()
{
	cout<<"free StateMachineImp"<<endl;
	if(runThread != NULL)
		delete runThread;
}

void StateMachineImp::run()
{
	stop_flag = false;
	runThread = new thread(&StateMachineImp::handleMsg, this);
}

void StateMachineImp::stop()
{
	stop_flag = true;
	runThread->join();
}

void StateMachineImp::handleMsg()
{
	cout<<"start handle msg"<<endl;
	while(!stop_flag)
	{
		MsgType msg = msgQ->pop(3);
		if( msg.sock < 0)
			continue;
		else
		{
			if( msg.msg == "" )
			{
				cout<<"send hello"<<endl;
				string hello;
				hello += "<hello xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\n";
				hello += "     <capabilities>\n";
				hello += "       <capability>urn:ietf:params:netconf:base:1.1</capability>\n";
				hello += "     </capabilities>\n";
				hello += "     <session-id>1</session-id>\n";
				hello += "</hello>]]>]]>\n";

				socketS->sendMsg(msg.sock, hello);
			}
			else if( msg.msg.find("<close-session>")!=string::npos || msg.msg.find("<kill-session>")!=string::npos)
			{
				//TODO send rsp
				cout<<"close session"<<endl;
				socketS->closeSocket(msg.sock);
			}
			else
			{
				cout<<"echo session"<<endl;
				cout<<msg.msg<<endl;
				socketS->sendMsg(msg.sock, msg.msg);
			}
		}
	}
	
	cout<<"exit from state machine thread"<<endl;
}

