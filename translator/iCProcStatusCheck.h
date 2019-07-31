#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;
class iCProgram;

//=================================================================================================
//Process active/inactive check expression
//=================================================================================================
class iCProcStatusCheck : public iCExpression
{
	bool proc_is_proctype_param = false;
	const iCProcTypeParam *proctype_param = NULL; //where to take value from if started_proc_name is proctype param
public:
	std::string proc_name;
	bool active;
	const iCProgram* program;
	iCProcStatusCheck(const std::string& proc_name, bool active, const ParserContext& context);
	virtual void gen_code(CodeGenContext& context);
	void second_pass();

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCProcStatusCheck";}
#endif//DEBUG
};