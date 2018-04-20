#include "iCResetTimeoutStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
//#include "iCProgram.h"
#include "iCProcess.h"

//=================================================================================================
//Semantic checks
//=================================================================================================
void iCResetTimeoutStatement::second_pass()
{
	/*
	//check that process was defined
	proc = program->find_proc(proc_name);
	if(NULL == proc)
	{
		err_msg("undefined process %s", proc_name.c_str());
	}
	else if(in_isr)
	{
		proc->mark_isr_referenced();
	}*/
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCResetTimeoutStatement::gen_code(CodeGenContext& context)
{
	if(NULL == proc)
		return;

	bool need_atomic_block = !context.in_ISR() && (proc->is_isr_driven() || proc->is_isr_referenced());

	//add atomic block if in background loop
	if(need_atomic_block)
		context.atomic_header();

	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("%s(%s);", C_RESET_T_MACRO, proc_name.c_str());

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
	//program(context.get_program()),
	iCNode(context)//,
	//in_isr(context.in_isr())
{
	proc = context.get_process();
	if(NULL == proc)
		err_msg("\"reset timeout\" used outside any process");
	else
		proc_name = proc->name;

	line_num = context.line();
}