#include "iCProcess.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCState.h"

int iCProcess::wcet()
{
	std::cout<<"iCProcess::wcet"<<std::endl;
	int max_w = 0;
	for(iCStateList::iterator i=states.begin();i!=states.end();i++)
	{
		if(NULL != *i)
		{
			int sw = (*i)->wcet();
			if(max_w < sw)
				max_w = sw;
		}
	}
	std::cout<<"iCProcess \n"<<max_w<<std::endl;//debaga
	return max_w;
}

std::vector<iCNode*> iCProcess::get_issues()
{
	std::vector<iCNode*> issues;// = static_cast< std::vector<iCNode*> >(block_items);
	for(iCStateList::iterator i=states.begin();i!=states.end();i++)
	{
		issues.push_back(*i);
	}
	return issues;
}

/*
//=================================================================================================
//Alternative code generator - generates process code as an inline function
//=================================================================================================
void iCProcess::gen_code_func(CodeGenContext& context)
{
	//update context
	context.process = this;

	//Add comments
	context.to_code_fmt("%s\n", C_COMMENT_FRAME);
	context.to_code_fmt("//Process: %s\n", name.c_str());
	context.to_code_fmt("%s\n", C_COMMENT_FRAME);
	
	//Add pre comment
	context.to_code_fmt("%s", pre_comment.c_str());
	
	//Function header
	context.to_code_fmt("inline void %s()\n{\n", name.c_str());
	context.indent_depth++;
	
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
	
	//Function footer
	context.indent_depth--;
	context.to_code_fmt("}\n");

	//update context
	context.process = NULL;
}
*/

//=================================================================================================
//Code generator
//=================================================================================================
void iCProcess::gen_code(CodeGenContext& context)
{
	//std::cout << "iCProcess::gen_code " << name << std::endl;//debug
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
	
	if(!pre_comment.empty() && context.retain_comments)
	{
		//Add pre comment
		context.to_code_fmt("%s", pre_comment.c_str());
	}
	
	if(context.procs_as_funcs)
	{
		//Function header
		context.to_code_fmt("inline void %s_proc()\n{\n", name.c_str());
		context.indent_depth++;
	}
	
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

	if(context.procs_as_funcs)
	{
		//Function footer
		context.indent_depth--;
		context.to_code_fmt("}\n");
	}

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

	if(context.procs_as_funcs)
	{
		//Function header
		context.to_code_fmt("inline void %s_timeout()\n{\n", name.c_str());
		context.indent_depth++;
	}

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
	
	if(context.procs_as_funcs)
	{
		//Function footer
		context.indent_depth--;
		context.to_code_fmt("}\n");
	}

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
	
	//appropriate the currently pending pre comment
	pre_comment = const_cast<ParserContext&>(context).grab_pre_comment();
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
