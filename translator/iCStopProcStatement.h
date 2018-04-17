#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;
class iCProgram;

//=================================================================================================
//IndustrialC "start process proc_name;" statement
//=================================================================================================
class iCStopProcStatement : public iCStatement
{
	bool in_isr;
public:
	std::string proc_name;
	const iCProgram* program; // does not own
	iCStopProcStatement(const std::string& proc_name, const ParserContext& context);
	virtual ~iCStopProcStatement() {}
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCStopProcStatement";}
#endif//DEBUG
};


