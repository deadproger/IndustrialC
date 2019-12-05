#pragma once
#include "iCNode.h"

class ParserContext;
class CodeGenContext;

//=================================================================================================
//C jump statement
//Jump statements are goto, break, continue & return
//=================================================================================================
class iCJumpStatement : public iCStatement
{
	std::string op;
	iCExpression* expr; // owns
public:
	iCJumpStatement(const std::string& op, iCExpression* expr, const ParserContext& context);
	virtual ~iCJumpStatement();

	virtual void gen_code(CodeGenContext& context);
	
	virtual ICNODE_TYPE node_type() { return ICJUMPSTATEMENT; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCJumpStatement";}
#endif//DEBUG
};