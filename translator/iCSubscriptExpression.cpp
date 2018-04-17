#include "iCSubscriptExpression.h"
#include "ParserContext.h"
#include "CodeGenContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCSubscriptExpression::gen_code(CodeGenContext& context)
{	
	if(NULL == postfix_expr || NULL == constant_expr) 
		std::cout<<"Error: NULL expression in subscript"<<std::endl;

	context.set_location(line_num, filename);
	postfix_expr->gen_code(context);
	context.to_code("[");
	constant_expr->gen_code(context);
	context.to_code("]");
}

//=================================================================================================
//
//=================================================================================================
iCSubscriptExpression::~iCSubscriptExpression()
{
	delete postfix_expr;
	delete constant_expr;
}
