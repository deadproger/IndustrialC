#include "iCSelectionStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//
//=================================================================================================
iCSelectionStatement::iCSelectionStatement( const ParserContext& context, iCStatement *body, iCExpression *expr )
	:	iCNode(context),
		body(body),
		expr(expr)
{
	if(NULL != expr)
		line_num = expr->line_num;
}

//=================================================================================================
//
//=================================================================================================
void iCIfElseStatement::gen_code( CodeGenContext& context )
{
	//context.to_code_fmt("\n");
	context.set_location(line_num, filename);

	//header
	context.indent();
	context.to_code_fmt("if(");
	expr->gen_code(context);
	context.to_code_fmt(")\n");

	//if body
	if(!body->is_compound()) context.indent_depth++;
	body->gen_code(context);
	if(!body->is_compound()) context.indent_depth--;

	//else body
	if(NULL != else_body)
	{
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("else\n");
		if(!else_body->is_compound()) context.indent_depth++;
		else_body->gen_code(context);
		if(!else_body->is_compound()) context.indent_depth--;
	}
}

//=================================================================================================
//
//=================================================================================================
void iCSwitchStatement::gen_code( CodeGenContext& context )
{
	//context.to_code_fmt("\n");
	context.set_location(line_num, filename);

	//header
	context.indent();
	context.to_code_fmt("switch(");
	expr->gen_code(context);
	context.to_code_fmt(")\n");

	//if body
	if(!body->is_compound()) context.indent_depth++;
	body->gen_code(context);
	if(!body->is_compound()) context.indent_depth--;
}

//=================================================================================================
//
//=================================================================================================
void iCCaseStatement::gen_code( CodeGenContext& context )
{
	//context.to_code_fmt("\n");
	context.set_location(line_num, filename);

	//header
	context.indent();
	context.to_code_fmt("case ");
	expr->gen_code(context);
	context.to_code_fmt(" : ");

	// body
	if(!body->is_compound()) context.indent_depth++;
	body->gen_code(context);
	if(!body->is_compound()) context.indent_depth--;
}

//=================================================================================================
//
//=================================================================================================
void iCDefaultStatement::gen_code( CodeGenContext& context )
{
	//context.to_code_fmt("\n");
	context.set_location(line_num, filename);

	//header
	context.indent();
	context.to_code_fmt("default:");

	// body
	if(!body->is_compound()) context.indent_depth++;
	body->gen_code(context);
	if(!body->is_compound()) context.indent_depth--;
}

//=================================================================================================
//
//=================================================================================================
iCSelectionStatement::~iCSelectionStatement()
{	
	delete expr;
	delete body;
}

//=================================================================================================
//
//=================================================================================================
iCIfElseStatement::~iCIfElseStatement()
{	
	delete else_body;
}