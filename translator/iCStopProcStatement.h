#pragma once


#include "iCNode.h"
class CodeGenContext;
class ParserContext;
class iCProgram;

class iCStopProcStatement : public iCStatement
{
public:
	std::string proc_name;
	const iCProgram* program;
	iCStopProcStatement(const std::string& proc_name, const ParserContext& context);
	virtual ~iCStopProcStatement() {}
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();
};


