#include "iCTimeout.h"
#include "CodeGenContext.h"
#include "iCProcess.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCTimeout::gen_code( CodeGenContext& context )
{
	if(context.in_ISR())
	{
		//context.to_code_fmt("\n");
		context.set_location(line_num, filename);

		//timeout header
		context.indent();
		context.to_code_fmt("timeout(%s, ", context.process->name.c_str());
		period->gen_code(context);
		context.to_code_fmt(")\n");
	}
	else
	{
		//atomically fetch the process's time
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_START);
		context.indent_depth++;
		context.indent();

		context.to_code_fmt("%s = %s[%s].%s;",	C_COMMON_BKG_PROC_TIME,
												C_PROC_ARRAY_NAME,
												context.process->name.c_str(),
												C_STATE_TIME_ATTR_NAME);
		context.to_code_fmt("\n");
		context.indent_depth--;
		context.indent();
		context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_END);

		//context.to_code_fmt("\n");
		context.set_location(line_num, filename);

		//timeout header
		context.indent();
		context.to_code_fmt("//Timeout\n");
		context.indent();
		context.to_code_fmt("if((%s - %s) >= ",C_SYS_TIME_CUR_NAME, C_COMMON_BKG_PROC_TIME);
		period->gen_code(context);
		context.to_code_fmt(")\n");
	}

	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	//push statement nodes in stack in reverse order
	for(iCBlockItemsList::iterator i=body.begin();i!=body.end();i++)
	{
		(*i)->gen_code(context);
	}

	//timeout footer
	context.indent_depth--;
	context.to_code_fmt("\n");
	context.indent();
	context.to_code_fmt("}");
}

//=================================================================================================
//
//=================================================================================================
iCTimeout::~iCTimeout()
{
	for(iCBlockItemsList::iterator i=body.begin();i!=body.end();i++)
	{
		delete *i;
	}
	if(NULL != period)
		delete period;
}