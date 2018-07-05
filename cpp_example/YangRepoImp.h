#ifndef _YANG_REPO_IMP
#define _YANG_REPO_IMP

#include "YangRepoIF.h"

using namespace std;

class YangRepoImp: public YangRepoIF
{
public:
	YangRepoImp() {}
	virtual ~YangRepoImp();

private:
	list<Statement*> tops;

	virtual void add(Statement* top);
	virtual void del(Statement* top);
	void freeStatement(Statement* st);
        virtual const list<Statement*>& getTopStatements();
};


#endif
