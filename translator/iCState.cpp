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
	std::cout << "iCState::gen_code " << name << std::endl;
#ifdef ICDEBUG_TRACE
	std::cout<<"iCState::gen_code " << name << "...";
	std::cout.flush();
#endif

	//todo: use same parent for process and proctype instantiation?
	std::string state_name;
	if (special)
	{
		state_name = name;
	} else
	{
		if (NULL != context.process)
			state_name = context.process->name + name;
		else
			state_name = context.proctype_instantiation->name + name;
	}
	
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

//=================================================================================================
//Generates only the timeout code
//Used for ISR-driven processes' timeout checks in background
//=================================================================================================
void iCState::gen_timeout_code( CodeGenContext& context )
{
#ifdef ICDEBUG_TRACE
	std::cout<<"iCState::gen_timeout_code " << name << "...";
	std::cout.flush();
#endif

	std::string state_name = special ? name : (context.process->name + name);

	//update context
	context.state = this;

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

/*iCState::iCState(const iCState& state)
{
	//iCNode fields
	this->line_num = state.line_num;
	this->col_num = state.col_num;
	this->filename = state.filename;

	//iCState fields
	this->timeout = state.timeout; //ptr
	this->isr_driven = state.isr_driven;
	this->special = state.special;
	this->name = state.name;
	this->block_items = iCBlockItemsList();
	for (iCBlockItemsList::const_iterator i = state.block_items.begin(); i < state.block_items.end(); i++)
	{
		this->block_items.push_back(*i); //ptr
	}
}*/

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

