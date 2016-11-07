#include "iCBinaryExpression.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCBinaryExpression::gen_code(CodeGenContext& context)
{	
	if(NULL == left || NULL == right) {return;}

	context.set_location(line_num, filename);
	left->gen_code(context);
	context.to_code_fmt(" %s ", op.c_str());
	right->gen_code(context);
}

//=================================================================================================
//
//=================================================================================================
iCBinaryExpression::~iCBinaryExpression()
{
	delete left;
	delete right;
}
