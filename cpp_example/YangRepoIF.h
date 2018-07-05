#ifndef _YANG_REPO_IF
#define _YANG_REPO_IF

#include <string>
#include <list>
#include "YangParserIF.h"

using namespace std;

class YangRepoIF
{
public:
	YangRepoIF() {}
	virtual ~YangRepoIF() {}

	virtual void add(Statement* top)=0;
	virtual void del(Statement* top)=0;

	virtual const list<Statement*>& getTopStatements() = 0;

	//virtual void findTVP(E_YK_TYPE t, const string& value, const string& prefix, )=0;
	//virtual void findKVP(E_YK_TYPE t, const string& value, const string& prefix)=0;
};


#endif
