#pragma once

#include "iCNode.h"
class CodeGenContext;
class iCProgram;
class iCHyperprocess;
class ParserContext;

class iCStartHPStatement : public iCStatement
{
public:
	std::string hp_name;
	const iCProgram* program; 
	const iCHyperprocess* hp;
	iCStartHPStatement(const std::string& hp_name, const ParserContext& context);
	virtual ~iCStartHPStatement() {}
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();
};
