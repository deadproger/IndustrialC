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
	iCExpressionStatement(iCExpression* expr, const ParserContext& context);
	virtual ~iCExpressionStatement();
	virtual void gen_code(CodeGenContext& context);
	
	virtual int wcet() 
	{	std::cout<<"iCExpressionStatement\n";//debaga
		return 1;
	}//STUB
	
	virtual ICNODE_TYPE node_type() { return ICEXPRESSIONSTATEMENT; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCExpressionStatement";}
#endif//DEBUG
};