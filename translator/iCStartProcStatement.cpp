#include "iCStartProcStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCProgram.h"

//=================================================================================================
//Semantic checks
//=================================================================================================
void iCStartProcStatement::second_pass()
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
void iCStartProcStatement::gen_code(CodeGenContext& context)
{
	//add atomic block if in background loop
	if(!context.in_ISR())
	{
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_START);
		context.indent_depth++;
		context.indent();
	}

	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("%s(%s);\n", C_STARTPROC_MACRO, proc_name.c_str());

	//atomic block footer
	if(!context.in_ISR())
	{
		context.to_code_fmt("\n");
		context.indent_depth--;
		context.indent();
		context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_END);
	}
}

//=================================================================================================
//
//=================================================================================================
iCStartProcStatement::iCStartProcStatement( const std::string& proc_name, const ParserContext& context )
	:	proc_name(proc_name),
		program(context.get_program()),
		iCNode(context)
{
	line_num = context.line();
}