#include "iCState.h"
#include "CodeGenContext.h"
#include "iCProcess.h"
#include "ParserContext.h"
#include "iCTimeout.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCState::gen_code(CodeGenContext& context)
{
#ifdef ICDEBUG_TRACE
	std::cout<<"iCState::gen_code " << name << "...";
	std::cout.flush();
#endif


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

	for(iCBlockItemsList::iterator i=block_items.begin();i!=block_items.end();i++)
	{
		if(NULL != *i)
			(*i)->gen_code(context);
	}

	//timeout statement
	//background-run states check & execute their timeout themselves
	//timeouts for isr-driven states are checked & executed in background by the time service
	if(NULL != timeout && !isr_driven)
	{
		timeout->gen_code(context);
	}

	//state footer
	context.indent_depth--;
	context.to_code_fmt("\n");
	context.indent();
	context.to_code_fmt("} //state %s\n", state_name.c_str());
	context.indent();
	context.to_code_fmt("break;\n\n");

	//update context
	context.state = NULL;

#ifdef ICDEBUG_TRACE
	std::cout<<"done iCState\n";
	std::cout.flush();
#endif
}

void iCState::gen_timeout_code( CodeGenContext& context )
{
#ifdef ICDEBUG_TRACE
	std::cout<<"iCState::gen_timeout_code " << name << "...";
	std::cout.flush();
#endif

	std::string state_name = special ? name : (context.process->name + name);

	//update context
	context.state = this;

	//context.set_location(line_num, filename);

	//state header
	context.indent();
	context.to_code_fmt("case %s:\n", state_name.c_str());
	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	//timeout statement
	//background-run states check & execute their timeout themselves
	//timeouts for isr-driven states are checked & executed in background by the time service
	timeout->gen_code(context);

	//state footer
	context.indent_depth--;
	context.to_code_fmt("\n");
	context.indent();
	context.to_code_fmt("} //state %s\n", state_name.c_str());
	context.indent();
	context.to_code_fmt("break;\n\n");

	//update context
	context.state = NULL;
#ifdef ICDEBUG_TRACE
	std::cout<<"done iCState\n";
	std::cout.flush();
#endif
}


//=================================================================================================
//
//=================================================================================================
iCState::iCState( const std::string& name, const ParserContext& context ) 
	:	name(name),
		special (START_STATE_NAME == name || STOP_STATE_NAME == name),
		iCNode(context),
		isr_driven(false),
		timeout(NULL)
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
	if(NULL != timeout)
		delete timeout;
}

