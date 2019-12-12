#include "iCIterationStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//
//=================================================================================================
iCIterationStatement::iCIterationStatement(iCStatement* init,
										   iCStatement* condition,
										   iCExpression* increment,
										   iCStatement* body,
										   const ParserContext& context)
	:	iCNode(context),
		init(init),
		condition(condition),
		increment(increment),
		body(body)
{
	line_num = init->line_num;
	
	//appropriate the currently pending pre comment
	pre_comment = const_cast<ParserContext&>(context).grab_pre_comment();
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCIterationStatement::gen_code( CodeGenContext& context )
{
#ifdef ICDEBUG_TRACE
	std::cout<<"iCIterationStatement::gen_code ...";
	std::cout.flush();
#endif

	//Add pre comment
	if(!pre_comment.empty() && context.retain_comments)
	{
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("%s", pre_comment.c_str());
	}

	context.set_location(line_num, filename);

	//header
	context.indent();
	context.disable_indentation();
	context.to_code_fmt("for(");
	if(NULL != init)
		init->gen_code(context);
	if(NULL != condition)
		condition->gen_code(context);
	if(NULL != increment)
		increment->gen_code(context);
	context.to_code_fmt(")");
	context.enable_indentation();//restore indentation

	//body
	if(NULL != body)
		body->gen_code(context);

#ifdef ICDEBUG_TRACE
	std::cout<<"done iCIterationStatement\n";
	std::cout.flush();
#endif

}

//=================================================================================================
//
//=================================================================================================
iCIterationStatement::~iCIterationStatement()
{	
	if(NULL != init) delete init;
	if(NULL != condition) delete condition;
	if(NULL != increment) delete increment;
	if(NULL != body) delete body;
}
