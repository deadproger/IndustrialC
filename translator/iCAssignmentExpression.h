#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;

//=================================================================================================
//Assignment expression
//=================================================================================================
class iCAssignmentExpression : public iCExpression
{
private: 
	iCExpression* unary;
	iCExpression* assgn;
	std::string op;
public:
	iCAssignmentExpression(iCExpression* unary, const std::string& op, iCExpression* assgn, const ParserContext& context);
	~iCAssignmentExpression();
	virtual void gen_code(CodeGenContext& context);
	
	virtual ICNODE_TYPE node_type() { return ICASSIGNMENTEXPRESSION; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCAssignmentExpression";}
#endif//DEBUG
};