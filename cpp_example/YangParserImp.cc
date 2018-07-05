#include <iostream>
#include <algorithm>
#include <cctype>
#include <locale>

#include "Yang.h"
#include "YangParserImp.h"


YangParserImp::~YangParserImp()
{
	cleanup();
}

void YangParserImp::cleanup()
{
	//top will be free by external owner
	top = NULL;
	errors.clear();
}

void YangParserImp::reset(Statement* stmt)
{
	cleanup();
	top = stmt;
	line = 1;	
	brace = 0;
}

Statement* YangParserImp::getTopStatement()
{
	return top;
}

const list<string>& YangParserImp::go(const string& file)
{
	ifstream stream(file);

	Statement* st = new Statement();
	st->file = new string(file);

	reset(st);
	parseStatement(top, stream);
	string prefix = findPrefix();
	if(prefix == "")
		errors.push_back("No prefix for file:" + file);
	else
	{
		top->prefix = new string(prefix);
		setPrefix(top,top->prefix);
	}
	stream.close();
	return errors;
}

bool YangParserImp::skipComment(ifstream& stream, string& sb)
{
	char c = stream.get();
	if(c != EOF)
	{
		if(c == '/')
		{
			sb += c;
			string aline;
			getline(stream, aline);
			sb += aline;
			line++;
			return true;
		}
		else if(c == '*')
		{
			sb += c;
			while((c = stream.get() ) != EOF)
			{
				if(c == '\n')
					line++;
				sb += c;
				if(c == '/' && sb[sb.length()-2] == '*' )
					return true;
			}
		}
	}

	errors.push_back("Unexpected comment at line:" + to_string(line));
	return false;
}

bool YangParserImp::skipQuotation(char c, ifstream& stream, string& sb)
{
	char c2;
	while((c2 = stream.get() ) != EOF)
	{
		if(c2 == '\n')
			line++;
		sb += c2;
		if(c2 == c && sb[sb.length()-2] != '\\')
			return true;
	}
	
	errors.push_back("Unexpected quotation at line:" + to_string(line));
	return false;
}

static inline void trim(string& s)
{
	s.erase(s.begin(),find_if(s.begin(), s.end(), [](int ch){ return !isspace(ch); }));
	s.erase(find_if(s.rbegin(), s.rend(), [](int ch){ return !isspace(ch);}).base(),s.end());
}

static inline void splitNameValue(const string& text, string& name, string& value)
{
	string txt = text;
	trim(txt);

	for(int i=0; i<txt.length(); i++)
	{
		if( isspace(txt[i]) )
		{
			name = txt.substr(0,i);
			value = txt.substr(i,txt.length()-i);
			trim(value);
			return;
		}
	}
	name = txt;
}

bool YangParserImp::parseStatement(Statement* parent, ifstream& stream)
{
	string sb;
	char c;
	while((c = stream.get() ) != EOF)
	{
		if(c == '\n')
			line++;
		if(c == '/')
		{
			int oldline = line;
			int startPos = sb.length();
			sb += c;
			if(!skipComment(stream,sb))
				return false;
			Statement* st = new Statement();
			st->file = parent->file;
			st->line = oldline;
			st->text = sb;
			st->type = YK_COMMENT;
			trim(st->text);
			st->value = st->text;
			sb.erase(startPos, sb.length());
			parent->children.push_back(st);
			continue;
		}
		else if (c == '\'' || c == '\"')
		{
			sb += c;
			if(!skipQuotation(c, stream, sb))
				return false;
			continue;
		}

		if(c == '{')
		{
			Statement* st = new Statement();
			st->file = parent->file;
			st->line = line;
			st->text = sb;
			splitNameValue(sb, st->keyword, st->value);
			st->type = yk2type(st->keyword);
			sb.clear();
			parent->children.push_back(st);
			brace++;
			if(!parseStatement(st,stream))
				return false;
		}
		else if(c == '}')
		{
			brace--;
			trim(sb);
			if(sb.length() > 0)
			{
				errors.push_back("Unexpected content at line:" + to_string(line) + ", content="+sb);
				return false;
			}
		}
		else if(c == ';')
		{
			Statement* st = new Statement();
			st->file = parent->file;
			st->line = line;
			st->text = sb;
			splitNameValue(sb, st->keyword, st->value);
			st->type = yk2type(st->keyword);
			sb.clear();
			parent->children.push_back(st);
		}
		else
			sb += c;
	}
	if( brace == 0 )
		return true;

	errors.push_back("Unexpected end of file");
	return false;
}

string YangParserImp::findPrefix()
{
	for(Statement* pstmt: top->children)
	{
		if(pstmt->type == YK_MODULE)
		{
			for(Statement* child: pstmt->children)
			{
				if(child->type == YK_PREFIX)
					return child->value;
			}
		}
		else if(pstmt->type == YK_SUBMODULE)
		{
			for(Statement* child: pstmt->children)
			{
				if(child->type == YK_BELONGS_TO)
					return child->children.front()->value;
			}
		}
	}
	return "";
}

void YangParserImp::setPrefix(Statement* stmt, string* prefix)
{
	if(stmt!= NULL)
	{
		for(Statement* s: stmt->children)
		{
			s->prefix = prefix;
			setPrefix(s, prefix);	
		}
	}
}

