#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <memory>

#include "YangParserImp.h"
#include "YangRepoImp.h"
#include "MsgQueueImp.h"
#include "SocketServerImp.h"
#include "StateMachineImp.h"


using namespace std;

void usage()
{
	cout<<"parser -p <yang file path> [-h]"<<endl;
}

void listYangFiles(const string& path, list<string>& files)
{
	struct dirent *ptr;
	string postfix = ".yang";

	DIR* dir = opendir(path.c_str());
	while((ptr = readdir(dir)) != NULL)
	{
		string file = ptr->d_name;
		if(file.length() > postfix.length())
			if(file.compare(file.size()-postfix.size(),postfix.size(),postfix)==0 )
				files.push_back(path+"/"+file);
	}
}

void indent(int level)
{
	for(int i=0;i<level;i++)
		cout<<"  ";
}

void print(Statement* stmt, int level)
{
	if(stmt!= NULL)
	{
		indent(level);	
		cout<<*(stmt->file)<<":"<<stmt->line<<*(stmt->prefix)<<endl;
		indent(level);	
		cout<<type2yk(stmt->type)<<"="<<stmt->value<<endl;
		for(Statement* s: stmt->children)
		{
			print(s, level+1);
		}
	}
}

int main(int argc, char* argv[])
{
	string path;
	int ch;
	
	unique_ptr<MsgQueueIF> q(new MsgQueueImp());
	unique_ptr<SocketServerIF> server(new SocketServerImp());
	unique_ptr<StateMachineIF> sm(new StateMachineImp(q.get(), server.get()));

	server->setup(q.get(), "127.0.0.1",12345);

	sm->run();
	server->run();	 //block here

	//chrono::milliseconds dura(1000);
	//this_thread::sleep_for(dura);
	//cout<<"stop..."<<endl;
	//h->stop();
	return 0;	

	while((ch = getopt(argc, argv, "p:h")) != EOF)
	{
		switch(ch)
		{
			case 'p': path = string(optarg); break;
			case 'h': usage(); break;
		}
	}

	if(path == "")
	{
		usage();
		exit(0);
	}
	
	list<string> files;
	listYangFiles(path, files);
	if(files.empty())
	{
		cout<<"No yang file in path:"<<path<<endl;
		exit(0);
	}
	

	YangRepoIF* repo = new YangRepoImp();
	YangParserIF* parser = new YangParserImp();

	for(string& file: files)
	{
		cout<<"loading "<<file<<endl;
		list<string> errors = parser->go(file);
		for(string& s: errors)
		{
			cout<<s<<endl;
		}
		repo->add(parser->getTopStatement());
	}

	const list<Statement*>& ls = repo->getTopStatements();

	for(Statement* st:ls)
	{
		print(st,0);	
	}

	delete parser;
	delete repo;

	return 0;
}
