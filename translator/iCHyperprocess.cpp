#include "iCHyperprocess.h"
#include "CodeGenContext.h"
#include "iCProcess.h"
#include "ParserContext.h"


int iCHyperprocess::wcet()
{
	//std::cout<<"iCHyperprocess::wcet"<<std::endl;
	int w=0;
	for(iCProcessMap::iterator i=procs.begin();i!=procs.end();i++)
	{
		if(NULL != i->second)
			w += (i->second)->wcet();
	}
	//std::cout<<"iCHyperprocess \n"<<w<<std::endl;//debaga
	return w;
}

std::vector<iCNode*> iCHyperprocess::get_issues()
{
	std::vector<iCNode*> issues;// = static_cast< std::vector<iCNode*> >(block_items);
	for(iCProcessMap::iterator i=procs.begin();i!=procs.end();i++)
	{
		issues.push_back(i->second);
	}
	return issues;
}

//=================================================================================================
//(procs_as_funcs) Generates a list of function defs for processes
//=================================================================================================
void iCHyperprocess::gen_proc_funcs( CodeGenContext& context )
{
	if(!context.procs_as_funcs)
		return;
	
	if(activator.empty())
	{
		std::cout<<"Empty activator in hyperprocess"<<std::endl;
		return;
	}
	bool background = ("background" == activator);
	if(!background)
		context.enter_ISR();
	
	context.to_code_fmt("\n");//just in case
	
	//processes
	for(iCProcessMap::iterator j=procs.begin();j!=procs.end();j++)
	{
		iCProcess* proc = j->second;
		proc->gen_code(context);
		if(proc->has_timeouts() && !background)
		{
			context.leave_ISR();
			proc->gen_timeout_code(context);
			context.enter_ISR();
		}
	}
	
	context.to_code_fmt("\n");//just in case
	
	if(!background)
		context.leave_ISR();
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCHyperprocess::gen_code( CodeGenContext& context )
{
	//std::cout << "iCHyperprocess: gen_code for " << this->activator << std::endl;//debug
#ifdef ICDEBUG_TRACE
	std::cout<<"iCHyperprocess::gen_code " << activator << "...";
	std::cout.flush();
#endif

	if(activator.empty())
	{
		std::cout<<"Empty activator in hyperprocess"<<std::endl;
		return;
	}

	bool background = ("background" == activator);

	//ISR header
	if(!background)
	{
		if(!pre_comment.empty() && context.retain_comments)
		{
			//Add pre comment
			context.to_code_fmt("%s", pre_comment.c_str());
			context.set_location(line_num, filename);
		}
		
		context.to_code_fmt("ISR(%s)\n{\n", int_vector.c_str());
		context.indent_depth++;
		context.enter_ISR();
	}

	if(!context.procs_as_funcs)
	{
		//processes
		for(iCProcessMap::iterator j=procs.begin();j!=procs.end();j++)
			j->second->gen_code(context);
	}
	else
	{
		//gen proc func calls instead of proc bodies
		for(iCProcessMap::iterator j=procs.begin();j!=procs.end();j++)
		{
			context.indent();
			context.to_code_fmt("%s_proc();\n", j->second->name.c_str());
		}
	}

	//ISR footer
	if(!background)
	{
		context.indent_depth--;
		context.to_code_fmt("}\n\n");
		context.leave_ISR();
	}

#ifdef ICDEBUG_TRACE
	std::cout<<"done iCHyperprocess\n";
	std::cout.flush();
#endif
}

//=================================================================================================
//Generates timeout-processing code in background for ISR-driven processes
//=================================================================================================
void iCHyperprocess::gen_timeout_code( CodeGenContext& context )
{
	for(iCProcessMap::iterator j=procs.begin();j!=procs.end();j++)
	{
		iCProcess* proc = j->second;
		if(proc->has_timeouts())
		{
			if(context.procs_as_funcs)
			{
				context.indent();
				context.to_code_fmt("%s_timeout();\n", proc->name.c_str());
			}
			else
			{
				proc->gen_timeout_code(context);
			}
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
		//std::cout<<"NULL process in hyperprocess add_proc"<<std::endl;//debug
		return;
	}
	procs[proc->name] = proc;
}

//=================================================================================================
//
//=================================================================================================
iCHyperprocess::iCHyperprocess( const std::string& activator, const ParserContext& context ) 
	: 	activator(activator),
		line_num(context.line()), 
		iCNode(context)
{
	//appropriate the currently pending pre comment
	pre_comment = const_cast<ParserContext&>(context).grab_pre_comment();
}

//=================================================================================================
//
//=================================================================================================
iCHyperprocess::~iCHyperprocess()
{
	for(iCProcessMap::iterator i=procs.begin();i!=procs.end();i++)
	{
		delete i->second;
	}
}
