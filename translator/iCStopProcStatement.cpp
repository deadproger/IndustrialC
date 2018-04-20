#include "iCStopProcStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCProgram.h"

//=================================================================================================
//
//=================================================================================================
void iCStopProcStatement::second_pass()
{
	//check that process was defined
	if(NULL == program->find_proc(proc_name))
		err_msg("undefined process %s", proc_name.c_str());
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCStopProcStatement::gen_code(CodeGenContext& context)
{
	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("%s(%s);", C_STOPPROC_MACRO, proc_name.c_str());
	/*if(in_isr)
		context.to_code_fmt("//in ISR");*/
	context.to_code_fmt("\n");
}

//=================================================================================================
//
//=================================================================================================
iCStopProcStatement::iCStopProcStatement( const std::string& proc_name, const ParserContext& context )
	:	proc_name(proc_name),
		program(context.get_program()),
		iCNode(context),
		in_isr(context.in_isr())//,
		//proc(NULL)
{
	line_num = context.line();
}
