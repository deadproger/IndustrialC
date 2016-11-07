#include "iCPrimaryExpression.h"
#include "CodeGenContext.h"
#include "ParserContext.h"


//=================================================================================================
//Code generator
//=================================================================================================
void iCPrimaryExpression::gen_code(CodeGenContext& context)
{	
	if(NULL == expr) {return;}
	
	context.set_location(line_num, filename);
	context.to_code_fmt("(");
	expr->gen_code(context);
	context.to_code_fmt(")");
}

//=================================================================================================
//
//=================================================================================================
iCPrimaryExpression::~iCPrimaryExpression()
{
	delete expr;
}
