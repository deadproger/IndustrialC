#include "iCStartProcStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCProgram.h"
#include "iCProcess.h"
#include "iCProcType.h"
#include "iCProcTypeParam.h"
#include "iCProcTypeParamUsage.h"

//=================================================================================================
//Semantic checks
//=================================================================================================
void iCStartProcStatement::second_pass()
{
	std::cout << "iCStartProcStatement::second_pass called, proc_is_proctype_param=" 
		<< proc_is_proctype_param << std::endl;

	if (!proc_is_proctype_param)
	{
		started_proc = program->find_proc(started_proc_name);

		//if it's parameterized process not from proctype params then check that process has been defined
		if (NULL == started_proc)
		{
			started_proc = program->find_proctype_instance(started_proc_name);
			if (NULL == started_proc)
			{
				err_msg("undefined process %s", started_proc_name.c_str());
			}
		}
	}
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCStartProcStatement::gen_code(CodeGenContext& context)
{
	if (proc_is_proctype_param)
	{
		started_proc = program->find_proc(proctype_param->original_value);
		//on gen_code stage all procs (parameterized and non-parameterized) are assuredly contained in program,
		//so there is no need to search proctype instances in program separately (in proctype_instantiations)
		if (NULL == started_proc)
		{
			err_msg("undefined process %s", proctype_param->original_value.c_str());
			return;
		}
		started_proc_name = proctype_param->original_value;
	}

	if (in_isr) //todo: fill in_isr if it's instance inside isr
	{
		started_proc->mark_isr_referenced();
	}

	bool need_atomic_block = !context.in_ISR() && (started_proc->is_isr_driven() || started_proc->is_isr_referenced());

	//add atomic block if in background loop
	if(need_atomic_block)
		context.atomic_header();

	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("%s(%s);", C_STARTPROC_MACRO, started_proc_name.c_str());

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
iCStartProcStatement::iCStartProcStatement( const std::string& proc_name, const ParserContext& context )
	:	started_proc_name(proc_name),
		program(context.get_program()),
		iCNode(context),
		in_isr(context.in_isr())
{
	//proctype params names have higher priority than global process names
	if (NULL != context.get_proctype())
	{
		iCProcTypeParamList params_list = context.get_proctype()->get_params();
		for (iCProcTypeParamList::iterator i = params_list.begin(); i != params_list.end(); i++)
		{
			if (0 == (*i)->name.compare(proc_name))
			{
				std::cout << "id " << proc_name << " is proctype param" << std::endl;
				proctype_param = *i;
				proc_is_proctype_param = true;
				break;
			}
		}
	}

	line_num = context.line();
}