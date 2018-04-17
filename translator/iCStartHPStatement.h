#pragma once

#include "iCNode.h"
class CodeGenContext;
class iCProgram;
class iCHyperprocess;
class ParserContext;

//=================================================================================================
//IndustrialC "start hyperprocess hp_name;" statement
//=================================================================================================
class iCStartHPStatement : public iCStatement
{
public:
	std::string hp_name;
	const iCProgram* program; // does not own
	const iCHyperprocess* hp; // does not own
	iCStartHPStatement(const std::string& hp_name, const ParserContext& context);
	virtual ~iCStartHPStatement() {}
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCStartHPStatement";}
#endif//DEBUG
};
