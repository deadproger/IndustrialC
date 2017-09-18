#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;
class iCProgram;

class iCStartProcStatement : public iCStatement
{
public:
	std::string proc_name; 
	const iCProgram* program;
	iCStartProcStatement(const std::string& proc_name, const ParserContext& context);
	virtual ~iCStartProcStatement(){}
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCStartProcStatement";}
#endif//DEBUG
};
