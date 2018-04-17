#pragma once

#include "iCNode.h"
class CodeGenContext;

class iCUnaryExpression : public iCExpression
{
private:
	iCExpression* expr;//owns
	std::string op;
public:
	iCUnaryExpression(const std::string& op, iCExpression* expr, const ParserContext& context) : op(op), expr(expr), iCNode(context) {}
	~iCUnaryExpression();
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCUnaryExpression";}
#endif//DEBUG
};
