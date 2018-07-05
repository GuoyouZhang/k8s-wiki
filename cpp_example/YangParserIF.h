#ifndef _YANG_PARSER_IF
#define _YANG_PARSER_IF

#include <string>
#include <list>
#include "Yang.h"

using namespace std;

//for each yang file, there will only be one top level statement, which is memory owner
struct Statement 
{
	int line;	//the line number in yang file
	string text;	//keyword and value
	E_YK_TYPE type;	//when YK_UNKNOWN is used, the keyword could be a extention name
	string keyword;	//could be empty if type has a normal value to save memory
	string value;	//empty for top level Statement
	string* file;	//point to same address for the same yang file, only the owner needs to free it
	string* prefix;	//point to same prefix for the same yang file, only the owner needs to free it
	list<Statement*> children;
};

class YangParserIF
{
public:
	YangParserIF() {}
	virtual ~YangParserIF() {}

	virtual const list<string>& go(const string& file)=0;
	virtual Statement* getTopStatement()=0;
};


#endif
