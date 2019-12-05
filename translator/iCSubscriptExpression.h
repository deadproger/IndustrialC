#pragma once

#include "iCNode.h"

class CodeGenContext;

//=================================================================================================
//Subscript(array indexing) expression 
//=================================================================================================
class iCSubscriptExpression : public iCExpression
{
private:
	iCExpression* postfix_expr;//owns
	iCExpression* constant_expr;//owns
public:
	iCSubscriptExpression(iCExpression* postfix,
						  iCExpression* constant,
						  const ParserContext& context)
			: postfix_expr(postfix),
			  constant_expr(constant),
			  iCNode(context){}
	~iCSubscriptExpression();
	virtual void gen_code(CodeGenContext& context);

	virtual ICNODE_TYPE node_type() { return ICSUBSCRIPTEXPRESSION; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCSubscriptExpression";}
#endif//DEBUG
};