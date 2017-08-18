#include "iCSelectionStatement.h"
#include "CodeGenContext.h"

iCSelectionStatement::iCSelectionStatement( const ParserContext& context )
	: iCNode(context),
	  body(NULL),
	  else_body(NULL)
{
	
}

void iCSelectionStatement::gen_code( CodeGenContext& context )
{
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

iCSelectionStatement::~iCSelectionStatement()
{	
	delete expr;
	delete body;
	delete else_body;
}