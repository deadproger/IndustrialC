#include "iCIterationStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

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
}

void iCIterationStatement::gen_code( CodeGenContext& context )
{
	//context.to_code_fmt("\n");
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


	if(NULL != body)
		body->gen_code(context);

}

iCIterationStatement::~iCIterationStatement()
{	
	if(NULL != init)
		delete init;
	if(NULL != condition)
		delete condition;
	if(NULL != increment)
		delete increment;
	if(NULL != body)
		delete body;
}
