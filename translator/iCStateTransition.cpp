#include "iCStateTransition.h"
#include "CodeGenContext.h"
#include "iCProcess.h"
#include "ParserContext.h"
#include "iCState.h"

//=================================================================================================
//Semantic checks
//=================================================================================================
void iCStateTransition::second_pass()
{
	//check that state has been defined
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
	//context.to_code_fmt("\n");
	context.set_location(line_num, filename);
	context.indent();
	
	bool need_atomic_block = !context.in_ISR() && (process->is_isr_driven() || process->is_isr_referenced());

	//add atomic block if in background loop
	if(need_atomic_block)
		context.atomic_header();

	context.to_code_fmt("%s(%s, ", C_STRANS_MACRO, context.process->name.c_str());
	if(START_STATE_NAME != state_name && STOP_STATE_NAME != state_name)
		context.to_code_fmt(context.process->name.c_str());
	context.to_code_fmt("%s);", state_name.c_str());

	//atomic block footer
	if(need_atomic_block)
		context.atomic_footer();
}

//=================================================================================================
//
//=================================================================================================
iCStateTransition::iCStateTransition( const std::string& state_name, const ParserContext& context )
	:	state_name(state_name),
		process(context.get_process()),
		iCNode(context)
{
	if(state_name == context.get_state()->name)
		warning_msg("state transition to the same state - use \"reset timeout;\" instead");
	
	line_num = context.line();
}

