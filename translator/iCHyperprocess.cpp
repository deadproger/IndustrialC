#include "iCHyperprocess.h"
#include "CodeGenContext.h"
#include "iCProcess.h"
#include "ParserContext.h"


//=================================================================================================
//Code generator
//=================================================================================================
void iCHyperprocess::gen_code( CodeGenContext& context )
{
	if(activator.empty())
	{
		std::cout<<"Empty activator in hyperprocess"<<std::endl;
		return;
	}

	bool background = (activator == "background");

	//ISR header
	if(!background)
	{
		context.to_code_fmt("ISR(%s)\n{\n", int_vector.c_str());
		context.indent_depth++;
		context.enter_ISR();
	}

	//processes
	for(iCProcessList::iterator j=procs.begin();j!=procs.end();j++)
	{
		j->second->gen_code(context);
	}

	//ISR footer
	if(!background)
	{
		context.indent_depth--;
		context.to_code_fmt("}\n\n");
		context.leave_ISR();
	}
}


void iCHyperprocess::gen_timeout_code( CodeGenContext& context )
{
	for(iCProcessList::iterator j=procs.begin();j!=procs.end();j++)
	{
		iCProcess* proc = j->second;
		if(proc->has_timeouts())
		{
			proc->gen_timeout_code(context);
		}
	}
}

//=================================================================================================
//
//=================================================================================================
void iCHyperprocess::add_proc(iCProcess* proc)
{
	if(NULL == proc)
	{
		std::cout<<"NULL process in hyperprocess add_proc"<<std::endl;
		return;
	}
	procs[proc->name] = proc;
}

//=================================================================================================
//
//=================================================================================================
iCHyperprocess::iCHyperprocess( const std::string& activator, const ParserContext& context ) : activator(activator), line_num(context.line()), iCNode(context)
{

}

//=================================================================================================
//
//=================================================================================================
iCHyperprocess::~iCHyperprocess()
{
	for(iCProcessList::iterator i=procs.begin();i!=procs.end();i++)
	{
		delete i->second;
	}
}
