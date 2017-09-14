#include "iCExpressionStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"


//=================================================================================================
//Code generator
//=================================================================================================
void iCExpressionStatement::gen_code(CodeGenContext& context)
{
	//context.to_code_fmt("\n");
	context.set_location(line_num, filename);
	if(NULL != expr)
	{
		context.indent();
		expr->gen_code(context);
	}
	context.to_code_fmt(";");
}

//=================================================================================================
//
//=================================================================================================
iCExpressionStatement::~iCExpressionStatement()
{
	if(NULL != expr)
		delete expr;
}