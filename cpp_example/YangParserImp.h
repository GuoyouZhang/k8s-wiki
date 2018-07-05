#ifndef _YANG_PARSER
#define _YANG_PARSER

#include <fstream>
#include "YangParserIF.h"

class YangParserImp: public YangParserIF
{
private:
	int line;
	int brace;
	Statement* top = NULL;
	list<string> errors;
public:
	YangParserImp() {}
	virtual ~YangParserImp();

private:
	const list<string>& go(const string& file);
	Statement* getTopStatement();

	void cleanup();
	void reset(Statement* stmt);
	bool parseStatement(Statement* parent, ifstream& stream);
	bool skipComment(ifstream& stream, string& sb);
	bool skipQuotation(char c, ifstream& stream, string& sb);
	string findPrefix();
	void setPrefix(Statement* stmt,string* prefix);
};


#endif
