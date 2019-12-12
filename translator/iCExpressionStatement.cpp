#include "iCExpressionStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCExpressionStatement::gen_code(CodeGenContext& context)
{
	//Add pre comment
	if(!pre_comment.empty() && context.retain_comments)
	{
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("%s", pre_comment.c_str());
	}
	
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

//=================================================================================================
//
//=================================================================================================
iCExpressionStatement::iCExpressionStatement( iCExpression* expr, const ParserContext& context ) : expr(expr), iCNode(context)
{
	line_num = context.line();
	
	//appropriate the currently pending pre comment
	pre_comment = const_cast<ParserContext&>(context).grab_pre_comment();
}