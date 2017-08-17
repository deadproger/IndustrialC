#pragma once

#include "iCNode.h"

class CodeGenContext;

//=================================================================================================
//Subscript(array indexing) expression class 
//=================================================================================================
class iCSubscriptExpression : public iCExpression
{
private:
	iCExpression* postfix_expr;
	iCExpression* constant_expr;
public:
	iCSubscriptExpression(iCExpression* postfix,
						  iCExpression* constant,
						  const ParserContext& context)
			: postfix_expr(postfix),
			  constant_expr(constant),
			  iCNode(context){}
	~iCSubscriptExpression();
	virtual void gen_code(CodeGenContext& context);
};