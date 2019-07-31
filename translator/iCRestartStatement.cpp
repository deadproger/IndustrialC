#include "iCRestartStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
//#include "iCProgram.h"
#include "iCProcess.h"

//=================================================================================================
//Semantic checks
//=================================================================================================
void iCRestartStatement::second_pass()
{
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCRestartStatement::gen_code(CodeGenContext& context)
{
	if(NULL == proc)
		return;

	bool need_atomic_block = !context.in_ISR() && (proc->is_isr_driven() || proc->is_isr_referenced());

	//add atomic block if in background loop
	if(need_atomic_block)
		context.atomic_header();

	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("%s(%s, %s);", C_STRANS_MACRO, proc_name.c_str(), START_STATE_NAME);

	context.to_code_fmt("\n");

	//atomic block footer
	if(need_atomic_block)
		context.atomic_footer();
}

//=================================================================================================
//
//=================================================================================================
iCRestartStatement::iCRestartStatement( const ParserContext& context )
:	iCNode(context)
{
	proc = context.get_process();
	if(NULL == proc)
		err_msg("\"restart\" used outside of process");
	else
		proc_name = proc->name;

	line_num = context.line();
}