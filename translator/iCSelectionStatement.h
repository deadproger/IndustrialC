#pragma once

#include "iCNode.h"
//#include "iCCompoundStatement.h"
class ParserContext;
class CodeGenContext;

//=================================================================================================
//C "if/else" statement
//=================================================================================================
class iCSelectionStatement : public iCStatement
{
	
public:
	iCStatement *body; // owns
	iCStatement *else_body; // owns
	iCExpression *expr; // owns
	iCSelectionStatement(const ParserContext& context);
	virtual ~iCSelectionStatement();
	void set_expression(iCExpression* expression)
	{
		expr = expression;
		line_num = expr->line_num;
	}

	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCSelectionStatement";}
#endif//DEBUG
};