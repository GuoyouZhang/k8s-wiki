#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include "SocketServerImp.h"

SocketServerImp::SocketServerImp()
{
	for(int i=0; i < MAX_CLIENT; i++)
	{
		peerfd[i].fd = -1;
	}
}

int SocketServerImp::setup(MsgQueueIF* q, const string& _ip,int _port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		return 2;
	}
	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(_port);
	local.sin_addr.s_addr = inet_addr(_ip.c_str());
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		return 3;
	}
	if(listen(sock,10) < 0)
	{
		return 4;
	}
	peerfd[0].fd = sock;
	peerfd[0].events = POLLIN;
	msgQ = q;
	return 0;
}

int SocketServerImp::sendMsg(int sock, const string& msg)
{
	int total = msg.size();
	size_t sent = 0;

	const char* buf = msg.c_str();

	while(sent < total)
	{
		size_t ret = write(sock, buf+sent, total-sent);
		if(ret <0)
		{
			for(int i=0; i<MAX_CLIENT;i++)
			{
				if(peerfd[i].fd == sock)
				{
					peerfd[i].fd = -1;	
					break;
				}
			}
			return ret;
		}
		sent += ret;
	}	
	return sent;
}

void SocketServerImp::closeSocket(int sock)
{
	map<int, string>::iterator it = buf_map.find(sock);
	if(it != buf_map.end())
		buf_map.erase(it);
	for(int i = 1;i < nfds;++i)
	{
		if(peerfd[i].fd == sock)
		{
			peerfd[i].fd = -1;	
			close(sock);
			return;
		}
	}
}

int SocketServerImp::readMsg(int sock)
{
	string msg;
	map<int, string>::iterator it = buf_map.find(sock);
	if(it != buf_map.end())
		msg = buf_map[sock];
	
	char buf[4096];
	ssize_t n = read(sock, buf, sizeof(buf)-1);
	if( n <= 0 )
	{
		if( it != buf_map.end() )
			buf_map.erase(it);
		return n;
	}
	buf[n]='\0';
	msg = msg + buf;
	cout<<msg<<endl;
	if( msg.length() > MSG_END.length() && msg.substr(msg.length()-MSG_END.length(), msg.length()-1) == MSG_END)
	{
		if( it != buf_map.end() )
			buf_map.erase(it);

		msgQ->push(sock, msg);
	}
	else //buf the data for unfinished msg
	{
		buf_map[sock] = msg;
	}
	return n;
}

void SocketServerImp::run()
{
	nfds = 1;
	int ret;
	int timeout = -1;
	while(1)
	{
		switch(ret = poll(peerfd,nfds,timeout))
		{
		case 0:
			printf("timeout...\n");
			break;
		case -1:
			perror("poll");
			break;
		default:
		{
			if(peerfd[0].revents & POLLIN)
			{
				struct sockaddr_in client;
				socklen_t len = sizeof(client);
				int new_sock = accept(peerfd[0].fd,(struct sockaddr*)&client,&len);
				printf("accept finish %d\n",new_sock);
				if(new_sock < 0)
				{
					perror("accept");
					continue;
				}
				printf("get a new client\n");
				int j = 1;
 				for(; j < MAX_CLIENT; ++j)
				{
					if(peerfd[j].fd < 0)
					{
						peerfd[j].fd = new_sock;
						break;
					}
				}
				if(j == MAX_CLIENT)
				{
					printf("to many clients...\n");
					close(new_sock);
				}
				else
				{
					msgQ->push(new_sock, "");
				}
				peerfd[j].events = POLLIN;
				if(j + 1 > nfds)
					nfds = j + 1;
			}
			for(int i = 1;i < nfds;++i)
			{
				if(peerfd[i].fd > 0 && peerfd[i].revents & POLLIN)
				{
					ret = readMsg(peerfd[i].fd);
					if(ret <= 0)
					{
						printf("close socket %d\n", peerfd[i].fd);
						close(peerfd[i].fd);
						peerfd[i].fd = -1;
					}
				}//i != 0
			}//for
		}//default
		break;
		}
	}
}
