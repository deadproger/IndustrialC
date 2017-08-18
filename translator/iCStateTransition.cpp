#include "iCStateTransition.h"
#include "CodeGenContext.h"
#include "iCProcess.h"
#include "ParserContext.h"


//=================================================================================================
//Semantic checks
//=================================================================================================
void iCStateTransition::second_pass()
{
	//check that state was defined
	ICASSERT(NULL != process);
	if(!process->has_state(state_name))
	{
		err_msg("undefined state %s", state_name.c_str());
	}
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCStateTransition::gen_code(CodeGenContext& context)
{
	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("%s(%s, ", C_STRANS_MACRO, context.process->name.c_str());
	if(START_STATE_NAME != state_name && STOP_STATE_NAME != state_name)
	{
		context.to_code_fmt(context.process->name.c_str());
	}
	context.to_code_fmt("%s);", state_name.c_str());
}

//=================================================================================================
//
//=================================================================================================
iCStateTransition::iCStateTransition( const std::string& state_name, const ParserContext& context )
	:	state_name(state_name),
		process(context.get_process()),
		iCNode(context)
{
	line_num = context.line();
}

