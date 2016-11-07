#pragma once

#include "iCNode.h"
class CodeGenContext;

class iCUnaryExpression : public iCExpression
{
private:
	iCExpression* expr;
	std::string op;
public:
	iCUnaryExpression(const std::string& op, iCExpression* expr, const ParserContext& context) : op(op), expr(expr), iCNode(context) {}
	~iCUnaryExpression();
	virtual void gen_code(CodeGenContext& context);
};
