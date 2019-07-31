#include "iCProcStatusCheck.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCProgram.h"
#include "iCProcType.h"
#include "iCProcTypeParam.h"
#include "iCProcTypeParamUsage.h"

//=================================================================================================
//Semantics checks
//=================================================================================================
void iCProcStatusCheck::second_pass()
{
	//check that process has been defined
	if (!proc_is_proctype_param && !program->proc_defined(proc_name) && !program->proctype_instance_defined(proc_name))
		err_msg("undefined process %s", proc_name.c_str());
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCProcStatusCheck::gen_code( CodeGenContext& context )
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

	context.set_location(line_num, filename);
	context.to_code_fmt("%s(%s)", (active?"check_active":"check_passive"), proc_name.c_str());
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