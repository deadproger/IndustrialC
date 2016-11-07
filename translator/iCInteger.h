#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//Integer constant
//=================================================================================================
class iCInteger : public iCExpression
{
public:
	std::string val;
	//unsigned long val;
	//iCInteger(int val, const ParserContext& context) : val(val), iCNode(context) {}
	iCInteger(const std::string& str, const ParserContext& context);
	virtual void gen_code(CodeGenContext& context);
};
