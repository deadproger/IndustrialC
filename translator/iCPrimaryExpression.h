#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//Primary expression
//=================================================================================================
class iCPrimaryExpression : public iCExpression
{
	iCExpression* expr;
public:
	iCPrimaryExpression(iCExpression* expr, const ParserContext& context) : expr(expr), iCNode(context) {}
	 ~iCPrimaryExpression();
	virtual void gen_code(CodeGenContext& context);
	
	virtual ICNODE_TYPE node_type() { return ICPRIMARYEXPRESSION; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCPrimaryExpression";}
#endif//DEBUG
};