#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//All binary expressions except assignment
//=================================================================================================
class iCBinaryExpression : public iCExpression
{
private:
	iCExpression* left;
	iCExpression* right;
	std::string op;
public:
	iCBinaryExpression(const ParserContext& context) : left(NULL), right(NULL), op(NULL), iCNode(context) {}
	iCBinaryExpression(iCExpression* left, const std::string& op, iCExpression* right, const ParserContext& context) : left(left), op(op), right(right), iCNode(context) {}
	~iCBinaryExpression();
	virtual void gen_code(CodeGenContext& context);
	
	virtual ICNODE_TYPE node_type() { return ICBINARYEXPRESSION; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCBinaryExpression op: " + op;}
#endif//DEBUG
};