#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;
//class iCProgram;

//=================================================================================================
//IndustrialC "reset timeout;" statement
//=================================================================================================
class iCResetTimeoutStatement : public iCStatement
{
	//bool in_isr;
public:
	std::string proc_name; 
	//const iCProgram* program; // does not own
	const iCProcess* proc; // does not own
	iCResetTimeoutStatement(const ParserContext& context);
	virtual ~iCResetTimeoutStatement(){}
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCStartProcStatement";}
#endif//DEBUG
};