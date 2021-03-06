#include "iCSelectionStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

int iCIfElseStatement::wcet()
{
	//body
	//else_body
	int w;
	int w1 = body->wcet();
	int w2 = else_body->wcet();
	if(w1 > w2)
		w = w1;
	else 
		w = w2;
	std::cout<<"iCIfElseStatement "<<w<<std::endl;//debaga
	return w;
}


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
	
	//appropriate the currently pending pre comment
	pre_comment = const_cast<ParserContext&>(context).grab_pre_comment();
}

//=================================================================================================
//
//=================================================================================================
void iCIfElseStatement::gen_code( CodeGenContext& context )
{
	//Add pre comment
	if(!pre_comment.empty() && context.retain_comments)
	{
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("%s", pre_comment.c_str());
	}
	
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