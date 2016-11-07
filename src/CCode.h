#pragma once 

#include "iCNode.h"
class CodeGenContext;
class CCodeLine;
class iCScope;
class ParserContext;
typedef std::vector<CCodeLine*> CCodeLinesList;

//=================================================================================================
//C code line
//=================================================================================================
class CCodeLine : public iCExpression
{
public:
	CCodeLine(const std::string& text,  ParserContext& parser_context);
	std::string text;
	virtual void gen_code(CodeGenContext& context);
};

//=================================================================================================
//C code block
//=================================================================================================
class CCode : public iCDeclaration, public iCExpression
{
	CCodeLinesList lines;
	const iCScope* scope;
public:
	CCode( ParserContext& parser_context);
	virtual ~CCode();
	void add_line(CCodeLine* line) {lines.push_back(line);}
	virtual void gen_code(CodeGenContext& context);
};