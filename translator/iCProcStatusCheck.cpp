#include "iCProcStatusCheck.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCProgram.h"

//=================================================================================================
//Semantics checks
//=================================================================================================
void iCProcStatusCheck::second_pass()
{
	//check that process was defined
	if(!program->proc_defined(proc_name))
	{
		err_msg("undefined process %s", proc_name.c_str());
	}
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCProcStatusCheck::gen_code( CodeGenContext& context )
{
	/*
	//add atomic block if in background loop
	if(!context.in_ISR())
	{
		context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_START);
		context.indent_depth++;
		context.indent();
	}
	*/

	context.set_location(line_num, filename);
	context.to_code_fmt("%s(%s)", (active?"check_active":"check_passive"), proc_name.c_str());

	/*
	//atomic block footer
	if(!context.in_ISR())
	{
		context.to_code_fmt("\n");
		context.indent_depth--;
		context.indent();
		context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_END);
	}
	*/
}

//=================================================================================================
//
//=================================================================================================
iCProcStatusCheck::iCProcStatusCheck( const std::string& proc_name, bool active, const ParserContext& context )
	:	proc_name(proc_name),
		active(active),
		program(context.get_program()),
		iCNode(context)
{
	line_num = context.line();
}