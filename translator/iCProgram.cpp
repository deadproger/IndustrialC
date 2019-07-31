#include "iCProgram.h"
#include "CodeGenContext.h"
#include "iCHyperprocess.h"
#include "iCProcType.h"
#include "iCProcess.h"
#include "iCState.h"
#include "iCFunction.h"
#include <sstream>

class ParserContext;

ParserContext* parser_context = NULL; //externed in parser.h

//=================================================================================================
//Code generator
//=================================================================================================
void iCProgram::gen_code(CodeGenContext& context)
{
	std::cout<<"iCProgram entered gen_code, procs size=" << procs.size() << std::endl;
	//stub definitions
	context.to_code_fmt("%s\n\n", C_STUB_DEFS);

	//routine definitions
	context.to_code_fmt("volatile unsigned long %s = 0;\n\n", C_SYS_TIME_CUR_NAME);

	//macro routines
	context.to_code_fmt("%s\n\n", C_MACRO_ROUTINES);

	//common background process time variable
	//used in atomic timeout detection
	context.to_code_fmt("unsigned long %s;\n", C_COMMON_BKG_PROC_TIME);

	//global declarations and c code
	for(iCDeclarationList::iterator i=mcu_decls.begin();i!=mcu_decls.end();i++)
		(*i)->gen_code(context);

	//variable declarations
	for(iCVariablesList::iterator i=var_list.begin();i!=var_list.end();i++)
		(*i)->gen_code(context);

	//function definitions
	for(std::list<iCFunction*>::iterator i=func_list.begin();i!=func_list.end();i++)
		(*i)->gen_code(context);

	//context.code<<std::endl;
	context.to_code_fmt("\n");

	//todo: remove this
    //proctypes definitions
    for (iCProctypeMap::iterator i = proctypes.begin(); i != proctypes.end(); i++)
    {
        i->second->gen_code(context);
    }

	//process names enumerator
	std::ostringstream proc_subroutines;
	context.to_code_fmt("enum %s\n{\n", C_PROC_ENUM_NAME);
	context.indent_depth++;
	for(iCProcessMap::iterator i=procs.begin();i!=procs.end();i++)
	{
		iCProcess* proc = i->second;
		const std::string& proc_name = proc->name;

		//process name enumerator
		context.indent();
		context.to_code_fmt("%s,\n", proc_name.c_str());	
	}
	context.indent();
	context.to_code_fmt("%s,\n};\n\n", C_PROC_ENUM_NUM);
	context.indent_depth--;
	
	//special state id constants
	context.to_code_fmt("const %s %s = 0;\n", C_STATE_FUNC_TYPE_NAME, STOP_STATE_NAME);
	context.to_code_fmt("const %s %s = 1;\n", C_STATE_FUNC_TYPE_NAME, START_STATE_NAME);
	context.to_code_fmt("\n");

	context.flush();

	for(iCProcessMap::iterator i=procs.begin();i!=procs.end();i++)
	{
		iCProcess* proc = i->second;
		const std::string& proc_name = proc->name;

		if(2 <= proc->states.size())//proc has states other than FS_START
		{
			//state names enumerators
			unsigned int state_id = 3;
			context.to_code_fmt("enum %s_STATES\n{\n", proc_name.c_str());
			for(iCStateList::const_iterator s=proc->states.begin();s!=proc->states.end();s++)
			{
				iCState& state = **s;
				if(!state.special)
				{
					std::string state_name = proc_name + state.name;
					context.to_code_fmt("\t%s=%d,\n", state_name.c_str(), state_id++);
				}
			}
			context.to_code_fmt("};\n\n");
		}
	}
	
	//process struct array
	context.to_code_fmt("typedef struct\n{\n");
	context.indent_depth++;
	context.indent();
	context.to_code_fmt("volatile  %s %s;\n", C_STATE_FUNC_TYPE_NAME, C_STATE_FUNC_ATTR_NAME);
	context.indent();
	context.to_code_fmt("volatile unsigned long T;\n}%s;\n", C_PROC_DATA_STRUCT_NAME);
	context.to_code_fmt("%s %s[%s];\n\n", C_PROC_DATA_STRUCT_NAME, C_PROC_ARRAY_NAME, C_PROC_ENUM_NUM);
	context.indent_depth--;

	//interrupt hyperprocesses
	for(iCHyperprocessMap::iterator i=hps.begin();i!=hps.end();i++)
	{
		iCHyperprocess* hp = i->second;
		if(NULL != hp)
		{
			if(0 == hp->procs.size())
			{
				std::cout<<"warning: hyperprocess "<< hp->activator <<" has no processes defined"<<std::endl;
			}
			if("background" != hp->activator)
				i->second->gen_code(context);
		}
	}

	//background hyperprocess
	context.to_code_fmt("int main()\n{\n");
	context.indent_depth++;

	//AVR specific
	context.indent();context.to_code_fmt("ic_ts_init();\n");
	context.indent();context.to_code_fmt("sei();\n");

	//start the first defined background process
	if(NULL == first_bkgrnd_process)
	{
		std::cout<<"error: at least one background process has to defined"<<std::endl;
		return;
	}
	context.indent();
	context.to_code_fmt("startp(%s);\n", /*hps["background"]->procs.begin()->second*/first_bkgrnd_process->name.c_str());
	context.indent(); context.to_code_fmt("while(1)\n");
	context.indent(); context.to_code_fmt("{\n");
	context.indent_depth++;

	//System time update
	context.to_code_fmt("\n");
	context.indent();
	context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_START);
	context.indent_depth++;
	context.indent();
	context.to_code_fmt("%s", C_SYS_TIME_UPDATE);
	context.to_code_fmt("\n");
	context.indent_depth--;
	context.indent();
	context.to_code_fmt("%s\n", C_ATOMIC_BLOCK_END);
	
	//time service: timeout instances for isr-driven processes
	for(iCHyperprocessMap::iterator i=hps.begin();i!=hps.end();i++)
	{
		iCHyperprocess* hp = i->second;
		if(NULL != hp)
		{
			if("background" != hp->activator)
			{
				hp->gen_timeout_code(context);
			}
		}
	}
	
	//code for background processes
	hps["background"]->gen_code(context);

	//program footer
	context.indent_depth--;
	context.indent(); context.to_code_fmt("} //while loop\n");
	context.indent_depth--;
	context.indent(); context.to_code_fmt("} //main\n");
}

//=================================================================================================
//
//=================================================================================================
iCProgram::~iCProgram()
{
	std::cout << "iCProgram destructor called" << std::endl;

	//clear the hyperprocesses
	for (iCHyperprocessMap::iterator i=hps.begin(); i!=hps.end(); i++)
		delete i->second;

    //clear proctypes
    for (iCProctypeMap::iterator i = proctypes.begin(); i != proctypes.end(); i++)
        delete i->second;

	//clear mcu declarations
	for (iCDeclarationList::iterator i=mcu_decls.begin(); i!=mcu_decls.end(); i++)
		delete *i;

	//clear functions
	for (std::list<iCFunction*>::iterator i=func_list.begin(); i!=func_list.end(); i++)
		delete *i;

	//clear variables
	for (iCVariablesList::iterator i=var_list.begin(); i!=var_list.end(); i++)
		delete *i;

	std::cout << "iCProgram: ended destructor" << std::endl;
}

void iCProgram::add_proctype(iCProcType* proctype)
{
    //printf("entered add_proctype\n");
    if (NULL == proctype)
    {
        std::cout << "iCProgram::add_proctype: NULL proctype" << std::endl;
        return;
    }
	if (proctype_defined(proctype->name))
	{
		printf("iCProgram::add_proctype: proctype %s already exists\n", proctype->name.c_str());
		delete proctype;
		return;
	}
	//proctypes.insert(proctype->name, proctype);
	proctypes[proctype->name] = proctype;
    //printf("proctypes size=%d\n", proctypes.size());
}

void iCProgram::add_proctype_instantiation(iCProcTypeInstantiation* instantiation)
{
	if (NULL == instantiation)
	{
		std::cout << "iCProgram::add_proctype_instantiation: NULL instantiation" << std::endl;
		return;
	}

	//todo: uncomment and check this
	/*if (NULL == first_bkgrnd_process && 0 == instantiation->activator.compare("background"))
		first_bkgrnd_process = instantiation;*/

	proctype_instantiations[instantiation->name] = instantiation;
}

//=================================================================================================
//
//=================================================================================================
void iCProgram::add_process( iCProcess* proc )
{
	printf("iCProgram entered add_process, name=%s\n", proc->name.c_str());
	//redefined process
	if(NULL == proc)
	{
		std::cout<<"iCProgram::add_process: NULL proc"<<std::endl;
		return;
	}

	//todo: what if all procs are parameterized?
	if(NULL == first_bkgrnd_process && 0==proc->activator.compare("background"))
		first_bkgrnd_process = proc;

	//add process to its respective hyperprocess
	iCHyperprocessMap::iterator i = hps.find(proc->activator);
	if(hps.end() == i)
	{
		delete proc;
		return;
	}

	i->second->add_proc(proc);
	procs[proc->name] = proc; // auxiliary list for quick checks
}

//=================================================================================================
//
//=================================================================================================
bool iCProgram::hp_defined( const std::string& activator )
{
	return hps.count(activator);
}

bool iCProgram::proctype_defined(const std::string& proctype_name)
{
	return proctypes.count(proctype_name);
}

const iCProcType* iCProgram::find_proctype(const std::string& proctype_name) const
{
	iCProctypeMap::const_iterator proctype = proctypes.find(proctype_name);
	if (proctypes.end() == proctype)
	{
		return NULL;
	}
	return proctype->second;
}

bool iCProgram::proctype_instance_defined(const std::string& instance_name) const
{
	return proctype_instantiations.end() != proctype_instantiations.find(instance_name);
}

const iCProcTypeInstantiation* iCProgram::find_proctype_instance(const std::string& instance_name) const
{
	iCProctypeInstantiationMap::const_iterator instance = proctype_instantiations.find(instance_name);
	if (proctype_instantiations.end() == instance)
	{
		return NULL;
	}
	return instance->second;
}

bool iCProgram::proc_defined(const std::string& proc_name) const
{
	return procs.end() != procs.find(proc_name);
}

const iCProcess* iCProgram::find_proc(const std::string& proc_name)const
{
	iCProcessMap::const_iterator proc = procs.find(proc_name);
	if (procs.end() == proc)
	{
		return NULL;
	}
	return proc->second;
}

//=================================================================================================
//
//=================================================================================================
void iCProgram::add_hyperprocess( iCHyperprocess* hp )
{
	if(hp_defined(hp->activator))
	{
		delete hp;
		return;
	}
	hps[hp->activator] = hp;
}

//=================================================================================================
//
//=================================================================================================
void iCProgram::add_mcu_declaration( iCDeclaration* decl )
{
	mcu_decls.push_back(decl);
}

//=================================================================================================
//
//=================================================================================================
void iCProgram::add_function( iCFunction* func )
{
	func_list.push_back(func);
}

//=================================================================================================
//
//=================================================================================================
const iCHyperprocess* iCProgram::get_hp( const std::string& hp_name ) const
{
	iCHyperprocessMap::const_iterator hp = hps.find(hp_name);
	if(hps.end() != hp)
		return hp->second;
	return NULL;
}
