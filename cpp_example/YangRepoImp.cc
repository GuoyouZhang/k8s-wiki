#include "YangRepoImp.h"

using namespace std;

YangRepoImp::~YangRepoImp()
{
	for(Statement* st: tops)
	{
		delete st->file;
		delete st->prefix;
		freeStatement(st);
	}
}

void YangRepoImp::freeStatement(Statement* st)
{
	for(Statement* child: st->children)
	{
		freeStatement(child);
	}
	delete st;
}

void YangRepoImp::add(Statement* top)
{
	tops.push_front(top);
}

void YangRepoImp::del(Statement* top)
{
	tops.remove(top);

	delete top->file;
	delete top->prefix;
	freeStatement(top);
}

const list<Statement*>& YangRepoImp::getTopStatements()
{
	return tops;
}


