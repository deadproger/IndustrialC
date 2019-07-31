#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;

//=================================================================================================
//IndustrialC "reset timeout;" statement
//=================================================================================================
class iCResetTimeoutStatement : public iCStatement
{
	//bool in_isr;
	bool in_proctype = false;
public:
	const iCProcess* proc; // does not own
	const iCProcType* proctype; // does not own
	iCResetTimeoutStatement(const ParserContext& context);
	virtual ~iCResetTimeoutStatement(){}
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCStartProcStatement";}
#endif//DEBUG
};