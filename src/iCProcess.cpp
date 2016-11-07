#include "iCProcess.h"
#include "CodeGenContext.h"
#include "iCState.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCProcess::gen_code(CodeGenContext& context)
{
	//update context
	context.process = this;

	context.set_location(line_num, filename);

	//process header
	context.indent();
	context.to_code_fmt("switch(%s[%s].%s)\n", C_PROC_ARRAY_NAME, name.c_str(), C_STATE_FUNC_ATTR_NAME);
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
iCNode(context)
{
	line_num = context.line();
}