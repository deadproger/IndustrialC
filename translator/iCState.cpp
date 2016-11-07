#include "iCState.h"
#include "CodeGenContext.h"
#include "iCProcess.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCState::gen_code(CodeGenContext& context)
{
	std::string state_name = special ? name : (context.process->name + name);

	//update context
	context.state = this;

	context.set_location(line_num, filename);

	//state header
	context.indent();
	context.to_code_fmt("case %s:\n", state_name.c_str());
	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	//push statement nodes in stack in reverse order
	//if(!block_items.empty())
	for(iCBlockItemsList::iterator i=block_items.begin();i!=block_items.end();i++)
	{
		//if(NULL != *i)
			(*i)->gen_code(context);
	}

	//state footer
	context.indent_depth--;
	context.indent();
	context.to_code_fmt("} //state %s\n", state_name.c_str());
	context.indent();
	context.to_code_fmt("break;\n\n");

	//update context
	context.state = NULL;
}

//=================================================================================================
//
//=================================================================================================
iCState::iCState( const std::string& name, const ParserContext& context ) 
	:	name(name),
		special (START_STATE_NAME == name || STOP_STATE_NAME == name),
		iCNode(context)
{
	line_num = context.line();
}

//=================================================================================================
//
//=================================================================================================
iCState::~iCState()
{
	for(iCBlockItemsList::iterator i=block_items.begin();i!=block_items.end();i++)
		delete *i;
}

