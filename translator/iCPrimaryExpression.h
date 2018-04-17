#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//Primary expression
//=================================================================================================
class iCPrimaryExpression : public iCExpression
{
private:
	iCExpression* expr;
public:
	iCPrimaryExpression(iCExpression* expr, const ParserContext& context) : expr(expr), iCNode(context) {}
	 ~iCPrimaryExpression();
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCPrimaryExpression";}
#endif//DEBUG
};