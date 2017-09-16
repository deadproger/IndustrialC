#include "iCProcess.h"
#include "CodeGenContext.h"
#include "iCState.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCProcess::gen_code(CodeGenContext& context)
{
#ifdef ICDEBUG_TRACE
	std::cout<<"iCProcess::gen_code " << name << "...";
	std::cout.flush();
#endif

	//update context
	context.process = this;

	context.to_code_fmt("%s\n", C_COMMENT_FRAME);
	context.to_code_fmt("//Process: %s\n", name.c_str());
	context.to_code_fmt("%s\n", C_COMMENT_FRAME);

	context.set_location(line_num, filename);

	//process header
	context.indent();
	//context.to_code_fmt("switch(%s[%s].%s)\n", C_PROC_ARRAY_NAME, name.c_str(), C_STATE_FUNC_ATTR_NAME);
	if(context.in_ISR())
	{
		//Example:
		//switch(psw[PROC_NAME].fsp)
		context.to_code_fmt("switch(%s[%s].%s)\n", C_PROC_ARRAY_NAME,
			name.c_str(), C_STATE_FUNC_ATTR_NAME);
	}
	else
	{
		//atomic fetching of the current state function
		//Example:
		//ATOMIC_BLOCK(ATOMIC_FORCEON){
		//	ic_common_bkg_state = psw[PROC_NAME].fsp;
		//}
		//switch(ic_common_bkg_state)
		context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_START);
		context.indent_depth++;
		context.indent();
		context.to_code_fmt("%s = %s[%s].%s;\n",	C_COMMON_BKG_FSP_NAME,
			C_PROC_ARRAY_NAME,
			name.c_str(),
			C_STATE_FUNC_ATTR_NAME);
		context.indent_depth--;
		context.indent();
		context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_END);
		context.indent();
		context.to_code_fmt("switch(%s)\n", C_COMMON_BKG_FSP_NAME);
	}
	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	//states
	for(StateList::iterator i=states.begin();i!=states.end();i++)
	{
		(*i)->gen_code(context);
	}

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
//Code generator for time service instance of the process - only states with timeouts, if any,
//and timeout code only
//=================================================================================================
void iCProcess::gen_timeout_code( CodeGenContext& context )
{
	//update context
	context.process = this;

	//context.set_location(line_num, filename);

	//process header
	context.indent();
	if(context.in_ISR())
	{
		//Example:
		//switch(psw[PROC_NAME].fsp)
		context.to_code_fmt("switch(%s[%s].%s)\n", C_PROC_ARRAY_NAME,
					name.c_str(), C_STATE_FUNC_ATTR_NAME);
	}
	else
	{
		//atomic fetching of the current state function
		//Example:
		//ATOMIC_BLOCK(ATOMIC_FORCEON){
		//	ic_common_bkg_state = psw[PROC_NAME].fsp;
		//}
		//switch(ic_common_bkg_state)
		context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_START);
		context.indent_depth++;
		context.indent();
		context.to_code_fmt("%s = %s[%s].%s;\n",	C_COMMON_BKG_FSP_NAME,
			C_PROC_ARRAY_NAME,
			name.c_str(),
			C_STATE_FUNC_ATTR_NAME);
		context.indent_depth--;
		context.indent();
		context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_END);
		context.indent();
		context.to_code_fmt("switch(%s)\n", C_COMMON_BKG_FSP_NAME);
	}
	
	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	//states - states with timeouts only
	for(StateList::iterator i=states.begin();i!=states.end();i++)
	{
		iCState* state = *i;
		if(state->has_timeout())
		{
			state->gen_timeout_code(context);
		}
		//(*i)->gen_code(context);
	}

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
	//std::cout<<"deleting process "<<name<<std::endl;
	for(StateList::iterator i=states.begin();i!=states.end();i++)
		delete *i;
	/*for(iCDeclarationList::iterator i=decls.begin();i!=decls.end();i++)
		delete *i;*/
}

//=================================================================================================
//Check process has a state by this name
//=================================================================================================
bool iCProcess::has_state( const std::string& state_name ) const
{
	//TODO: optimize
	for(StateList::const_iterator i=states.begin();i!=states.end();i++)
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
	iCNode(context)
{
	line_num = context.line();
}

void iCProcess::add_states( const StateList& states )
{
	this->states = states;
	if(isr_driven)
	{
		//pass isr_driven to states
		for(StateList::iterator i=this->states.begin();i!=this->states.end();i++)
		{
			iCState* state = *i;
			state->set_isr_driven();
			if(state->has_timeout())
			{
				_has_timeouts = true;
			}
		}
	}
}

/*void iCProcess::add_state( iCState* state )
{
	if(isr_driven)
	{
		state->set_isr_driven();
	}
	states.push_back(state);
}*/