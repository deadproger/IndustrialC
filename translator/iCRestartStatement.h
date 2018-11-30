#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;
//class iCProgram;

//=================================================================================================
//IndustrialC "restart;" statement
//=================================================================================================
class iCRestartStatement : public iCStatement
{
	//bool in_isr;
public:
	std::string proc_name; 
	//const iCProgram* program; // does not own
	const iCProcess* proc; // does not own
	iCRestartStatement(const ParserContext& context);
	virtual ~iCRestartStatement(){}
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCStartProcStatement";}
#endif//DEBUG
};