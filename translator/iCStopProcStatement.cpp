#include "iCStopProcStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCProgram.h"
#include "iCProcType.h"
#include "iCProcTypeParam.h"
#include "iCProcTypeParamUsage.h"

//=================================================================================================
//
//=================================================================================================
void iCStopProcStatement::second_pass()
{
	//if it's not stopping self process from proctype then check that process has been defined
	if(!proc_is_proctype_param && !proc_name.empty() && 
		!program->proc_defined(proc_name) && !program->proctype_instance_defined(proc_name))
	{
		err_msg("undefined process %s", proc_name.c_str());
	}
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCStopProcStatement::gen_code(CodeGenContext& context)
{
	context.set_location(line_num, filename);
	context.indent();
	if (stop_self_in_proctype) //if it's stopping self process from proctype when process instance name is unknown
	{
		context.to_code_fmt("%s(%s);", C_STOPPROC_MACRO, context.proctype_instantiation->name.c_str());
	}
	else
	{
		if (proc_is_proctype_param)
		{
			proc_name = proctype_param->original_value;
			//on gen_code stage all procs (parameterized and non-parameterized) are assuredly contained in program,
			//so there is no need to search proctype instances in program separately (in proctype_instantiations)
			if (NULL == program->find_proc(proc_name))
			{
				err_msg("undefined process %s", proc_name.c_str());
				return;
			}
		}

		context.to_code_fmt("%s(%s);", C_STOPPROC_MACRO, proc_name.c_str());
	}
	/*if(in_isr)
		context.to_code_fmt("//in ISR");*/
	context.to_code_fmt("\n");
}

//=================================================================================================
//for common case
//=================================================================================================
iCStopProcStatement::iCStopProcStatement( const std::string& proc_name, const ParserContext& context )
	:	proc_name(proc_name),
		program(context.get_program()),
		iCNode(context),
		in_isr(context.in_isr())//,
		//proc(NULL)
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

//for stopping self in proctype when instance name is unknown
iCStopProcStatement::iCStopProcStatement(const ParserContext& context)
	: program(context.get_program()),
	iCNode(context),
	in_isr(context.in_isr())
{
	stop_self_in_proctype = true;
	line_num = context.line();
}
