#pragma once

#include "iCNode.h"
#include "iCProcess.h"

class iCProgram;
class CodeGenContext;

class iCProcTypeInstantiation: public iCProcess
{
	iCProgram *program; // does not own
	const iCProcType *proctype; // does not own
	const std::string proctype_name;
	iCIdentifierList arg_list; //list of given arguments (owns)
public:
	iCProcTypeInstantiation(iCProgram* program, const std::string& proctype_name, const std::string& instance_name, 
		const iCIdentifierList& arg_list, const ParserContext& context);
	~iCProcTypeInstantiation();
	void second_pass();
	void gen_code(CodeGenContext& context);
};
