#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//
//=================================================================================================
class iCCastExpression : public iCExpression
{
private:
	
public:
	iCCastExpression(const ParserContext& context) : iCNode(context){}
	virtual void gen_code(CodeGenContext& context){}
};