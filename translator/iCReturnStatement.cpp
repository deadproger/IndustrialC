#include "iCReturnStatement.h"

#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//
//=================================================================================================
iCReturnStatement::iCReturnStatement(iCExpression* expr,
									 const ParserContext& context)
	: iCNode(context),
	expr(expr)
{
	
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCReturnStatement::gen_code( CodeGenContext& context )
{
#ifdef ICDEBUG_TRACE
	std::cout<<"iCReturnStatement::gen_code ...";
	std::cout.flush();
#endif

	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("return ");
	if(NULL != expr)
		expr->gen_code(context);
	context.to_code_fmt(";");

#ifdef ICDEBUG_TRACE
	std::cout<<"done iCReturnStatement\n";
	std::cout.flush();
#endif

}

//=================================================================================================
//
//=================================================================================================
iCReturnStatement::~iCReturnStatement()
{	
	if(NULL != expr)
		delete expr;
}

