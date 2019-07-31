#include "iCProcess.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCState.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCProcess::gen_code(CodeGenContext& context)
{
	std::cout << "iCProcess::gen_code " << name << std::endl;
#ifdef ICDEBUG_TRACE
	std::cout<<"iCProcess::gen_code " << name << "...";
	std::cout.flush();
#endif

	//update context
	context.process = this;

	//Add comments
	context.to_code_fmt("%s\n", C_COMMENT_FRAME);
	context.to_code_fmt("//Process: %s\n", name.c_str());
	context.to_code_fmt("%s\n", C_COMMENT_FRAME);
	context.set_location(line_num, filename);

	//Process header
	context.indent();
	context.to_code_fmt("switch(%s[%s].%s)\n", C_PROC_ARRAY_NAME, name.c_str(), C_STATE_FUNC_ATTR_NAME);
	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	//states
	for(iCStateList::iterator i=states.begin();i!=states.end();i++)
		(*i)->gen_code(context);

	//process footer
	context.indent_depth--;
	context.indent();
	context.to_code_fmt("}  //process %s\n\n", name.c_str());

	//update context
	context.process = NULL;

#ifdef ICDEBUG_TRACE
	std::cout<<"done iCProcess\n";
	std::cout.flush();
#endif
}


//=================================================================================================
//Code generator for time service instance of the process - only states that have timeouts,
//and only the timeout code form those states
//=================================================================================================
void iCProcess::gen_timeout_code( CodeGenContext& context )
{
	ICASSERT(!context.in_ISR())

	//update context
	context.process = this;

	context.to_code_fmt("%s\n", C_COMMENT_FRAME);
	context.to_code_fmt("//Process %s timeouts\n", name.c_str());
	context.to_code_fmt("%s\n", C_COMMENT_FRAME);

	//process header
	context.indent();
	context.to_code_fmt("switch(%s[%s].%s)\n", C_PROC_ARRAY_NAME, name.c_str(), C_STATE_FUNC_ATTR_NAME);
	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	//states - states with timeouts only
	for(iCStateList::iterator i=states.begin();i!=states.end();i++)
		if((*i)->has_timeout())
			(*i)->gen_timeout_code(context);

	//process footer
	context.indent_depth--;
	context.indent();
	context.to_code_fmt("}  //process %s\n\n", name.c_str());

	//update context
	context.process = NULL;
}

//=================================================================================================
//
//=================================================================================================
iCProcess::~iCProcess()
{
	for (iCStateList::iterator i = states.begin(); i != states.end(); i++)
		delete *i;
}

//=================================================================================================
//Check process has a state by this name
//=================================================================================================
bool iCProcess::has_state( const std::string& state_name ) const
{
	//TODO: optimize
	for(iCStateList::const_iterator i=states.begin();i!=states.end();i++)
		if(state_name == (*i)->name)
			return true;
	return false;
}

//=================================================================================================
//
//=================================================================================================
iCProcess::iCProcess( const std::string& name, const ParserContext& context ) :	name(name),
	start_state(NULL),
	stop_state(NULL),
	isr_driven(false),
	_has_timeouts(false),
	iCNode(context),
	isr_referenced(false)
{
	line_num = context.line();
}

//=================================================================================================
//
//=================================================================================================
void iCProcess::add_states( const iCStateList& states )
{
	this->states = states;
	if(isr_driven)
	{
		//pass isr_driven to states
		for(iCStateList::iterator i=this->states.begin(); i!=this->states.end(); i++)
		{
			iCState* state = *i;
			state->set_isr_driven();
			if(state->has_timeout())
				_has_timeouts = true;
		}
	}
}
