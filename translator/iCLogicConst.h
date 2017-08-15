#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//Integer constant
//=================================================================================================
class iCLogicConst : public iCExpression
{
public:
	bool val;
	//unsigned long val;
	//iCInteger(int val, const ParserContext& context) : val(val), iCNode(context) {}
	iCLogicConst(bool val, const ParserContext& context);
	virtual void gen_code(CodeGenContext& context);
};
