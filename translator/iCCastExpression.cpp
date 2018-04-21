#include "iCCastExpression.h"
#include "CodeGenContext.h"
//#include "ParserContext.h"

//=================================================================================================
//
//=================================================================================================
iCCastExpression::iCCastExpression( const std::string& type_name, iCExpression* expr, const ParserContext& context ) 
	:	iCNode(context),
		type_name(type_name), 
		expr(expr)
{

}

//=================================================================================================
//Code generator
//=================================================================================================
void iCCastExpression::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);
	context.to_code_fmt("(");
	context.to_code_string(type_name);	
	context.to_code_fmt(")");
	expr->gen_code(context);

}

//=================================================================================================
//
//=================================================================================================
iCCastExpression::~iCCastExpression()
{
	delete expr;
}