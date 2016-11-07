#include "iCPostfixExpression.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCPostfixExpression::gen_code(CodeGenContext& context)
{	
	if(NULL == expr) {return;}

	context.set_location(line_num, filename);
	expr->gen_code(context);
	context.to_code_fmt(op.c_str());
}

//=================================================================================================
//
//=================================================================================================
iCPostfixExpression::~iCPostfixExpression()
{
	delete expr;
}
