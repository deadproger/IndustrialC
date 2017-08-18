#include "iCTimeout.h"
#include "CodeGenContext.h"
#include "iCProcess.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCTimeout::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);

	//timeout header
	context.indent();
	context.to_code_fmt("timeout(%s, %d)\n", context.process->name.c_str(), period);
	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	//push statement nodes in stack in reverse order
	for(iCBlockItemsList::iterator i=body.begin();i!=body.end();i++)
	{
		(*i)->gen_code(context);
	}

	//state footer
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
}