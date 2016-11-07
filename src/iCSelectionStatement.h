#pragma once

#include "iCNode.h"
//#include "iCCompoundStatement.h"
class ParserContext;
class CodeGenContext;

class iCSelectionStatement : public iCStatement
{
	//iCBlockItemsList block_items;
	
public:
	iCStatement *body;
	iCStatement *else_body;
	iCExpression *expr;
	iCSelectionStatement(const ParserContext& context);
	virtual ~iCSelectionStatement();
	void set_expression(iCExpression* expression)
	{
		expr = expression;
		line_num = expr->line_num;
	}

	virtual void gen_code(CodeGenContext& context);
};