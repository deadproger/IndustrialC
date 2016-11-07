#include "iCExpressionStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"


//=================================================================================================
//Code generator
//=================================================================================================
void iCExpressionStatement::gen_code(CodeGenContext& context)
{
	context.set_location(line_num, filename);
	context.indent();
	expr->gen_code(context);
	context.to_code_fmt(";\n");
}

//=================================================================================================
//
//=================================================================================================
iCExpressionStatement::~iCExpressionStatement()
{
	delete expr;
}