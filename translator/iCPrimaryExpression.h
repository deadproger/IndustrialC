#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//
//=================================================================================================
class iCPrimaryExpression : public iCExpression
{
private:
	iCExpression* expr;
public:
	iCPrimaryExpression(iCExpression* expr, const ParserContext& context) : expr(expr), iCNode(context) {}
	~iCPrimaryExpression();
	virtual void gen_code(CodeGenContext& context);
};