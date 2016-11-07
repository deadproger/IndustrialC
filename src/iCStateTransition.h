#pragma once

#include "iCNode.h"
class CodeGenContext;
class iCProcess;
class ParserContext;

//=================================================================================================
//State transition
//=================================================================================================
class iCStateTransition : public iCStatement 
{
public:
	const iCProcess* process; // does not own
	std::string state_name;
	iCStateTransition(const std::string& state_name, const ParserContext& context);
	virtual ~iCStateTransition() {}
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();
};
