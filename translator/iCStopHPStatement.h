#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;
class iCProgram;

//=================================================================================================
//IndustrialC "stop hyperprocess hp_name;" statement
//=================================================================================================
class iCStopHPStatement : public iCStatement
{
public:
	std::string hp_name;
	const iCHyperprocess* hp; // does not own
	const iCProgram* program; // does not own

	iCStopHPStatement(const std::string& hp_name, const ParserContext& context);
	virtual ~iCStopHPStatement(){}
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();
	
	virtual ICNODE_TYPE node_type() { return ICSTOPHPSTATEMENT; }
};
