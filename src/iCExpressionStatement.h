#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//Expression statement
//=================================================================================================
class iCExpressionStatement : public iCStatement
{
public:
	iCExpression* expr; // Owns
	iCExpressionStatement(iCExpression* expr, const ParserContext& context) : expr(expr), iCNode(context){}
	virtual ~iCExpressionStatement();
	virtual void gen_code(CodeGenContext& context);
};