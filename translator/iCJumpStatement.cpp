#include "iCJumpStatement.h"

#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//
//=================================================================================================
iCJumpStatement::iCJumpStatement(const std::string& op, 
								 iCExpression* expr,
								 const ParserContext& context)
	:	iCNode(context),
		op(op),
		expr(expr)
{
	//appropriate the currently pending pre comment
	pre_comment = const_cast<ParserContext&>(context).grab_pre_comment();
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCJumpStatement::gen_code( CodeGenContext& context )
{
	//Add pre comment
	if(!pre_comment.empty() && context.retain_comments)
	{
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("%s", pre_comment.c_str());
	}
	
	context.set_location(line_num, filename);
	context.indent();
	context.to_code_string(op);
	context.to_code_fmt(" ");
	if(NULL != expr)
		expr->gen_code(context);
	context.to_code_fmt(";");
}

//=================================================================================================
//
//=================================================================================================
iCJumpStatement::~iCJumpStatement()
{	
	if(NULL != expr)
		delete expr;
}

