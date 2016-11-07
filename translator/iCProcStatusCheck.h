#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;
class iCProgram;

class iCProcStatusCheck : public iCExpression
{
public:
	std::string proc_name;
	bool active;
	const iCProgram* program;
	iCProcStatusCheck(const std::string& proc_name, bool active, const ParserContext& context);
	virtual void gen_code(CodeGenContext& context);
	void second_pass();
};