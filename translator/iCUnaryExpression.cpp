#include "iCUnaryExpression.h"
#include "CodeGenContext.h"
#include "ParserContext.h"


//=================================================================================================
//Code generator
//=================================================================================================
void iCUnaryExpression::gen_code(CodeGenContext& context)
{	
	if(NULL == expr) 
		return;
	context.set_location(line_num, filename);
	context.to_code_fmt(op.c_str());
	expr->gen_code(context);
}

//=================================================================================================
//
//=================================================================================================
iCUnaryExpression::~iCUnaryExpression()
{
	delete expr;
}

