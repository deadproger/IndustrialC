#include "iCResetTimeoutStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCProcess.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCResetTimeoutStatement::gen_code(CodeGenContext& context)
{
	if(in_proctype)
	{
		proc = context.proctype_instantiation;
	}

	bool need_atomic_block = !context.in_ISR() && (proc->is_isr_driven() || proc->is_isr_referenced());

	//add atomic block if in background loop
	if(need_atomic_block)
		context.atomic_header();

	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("%s(%s);", C_RESET_T_MACRO, proc->name.c_str());

	/*if(in_isr)
		context.to_code_fmt("//in ISR");*/
	context.to_code_fmt("\n");

	//atomic block footer
	if(need_atomic_block)
		context.atomic_footer();
}

//=================================================================================================
//
//=================================================================================================
iCResetTimeoutStatement::iCResetTimeoutStatement( const ParserContext& context )
:	//proc_name(proc_name),
	iCNode(context)//,
	//in_isr(context.in_isr())
{
	proc = context.get_process();
	if (NULL == proc)
	{
		in_proctype = true;
		if (NULL == proc && NULL == context.get_proctype())
		{
			err_msg("\"reset timeout\" used outside any process");
		}
	}
	
	line_num = context.line();
}