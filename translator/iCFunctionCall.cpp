#include "iCFunctionCall.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//
//=================================================================================================
iCFunctionCall::~iCFunctionCall()
{
	for(std::vector<iCExpression*>::iterator i=args.begin();i!=args.end();i++)
		delete *i;
}

//=================================================================================================
//
//=================================================================================================
void iCFunctionCall::gen_code( CodeGenContext& context )
{
	//Add pre comment
	if(!pre_comment.empty())
	{
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("%s", pre_comment.c_str());
	}
	
	context.set_location(line_num, filename);
	context.to_code(func_name);
	context.to_code("(");
	for(std::vector<iCExpression*>::iterator i=args.begin();i!=args.end();i++)
	{
		(*i)->gen_code(context);
		if(i != args.end()-1)
			context.to_code(", ");
	}
	context.to_code(")");
}

