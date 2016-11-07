#include "ParserContext.h"
#include "iCScope.h"
#include "iCVariable.h"

extern bool had_errors;

ParserContext::ParserContext()
	:	program(NULL),
		process(NULL),
		state(NULL),
		line_num(1),
		end_col(1),
		start_col(1),
		root_scope(NULL),
		current_scope(NULL)
{
	current_scope = root_scope = new iCScope("program");
}


ParserContext::~ParserContext()
{
	delete root_scope; //the scope tree recursively deletes itself (see iCScope destructor)
}

//=================================================================================================
//Scope routines
//=================================================================================================
void ParserContext::open_scope(const std::string& name)
{
	iCScope* scope = new iCScope(name);
	scope->prev_scope = current_scope;
	current_scope->issues.push_back(scope);
	current_scope = scope;
}

//=================================================================================================
//
//=================================================================================================
//void close_scope(iCNode* ast_node)
void ParserContext::close_scope()
{
	if(NULL == current_scope->prev_scope)
		return;
	current_scope = current_scope->prev_scope;
}

//=================================================================================================
//
//=================================================================================================
void ParserContext::add_var_to_scope(iCVariable* decl)
{
	current_scope->vars.push_back(decl);
}

void ParserContext::add_state_to_scope(const std::string& name)
{
	current_scope->states.insert(name);
}

void ParserContext::add_proc_to_scope(const std::string& name)
{
	current_scope->processes.insert(name);
}

//=================================================================================================
//
//=================================================================================================
/*bool ParserContext::check_scope(const std::string& identifier)const
{
	iCScope* scope = current_scope;

	while(NULL != scope)
	{
		//simple search for now - replace with a map or with binary search
		for(std::vector<iCVariableDeclaration*>::iterator i=scope->vars.begin();i!=scope->vars.end();i++)
		{
			iCStringList& var_names = (*i)->var_names;
			for(iCStringList::iterator j=var_names.begin();j!=var_names.end();j++)
			{
				if(0 == (*j)->compare(identifier))
				{
					return true;
				}
			}
		}

		//go up the scope tree
		scope = scope->prev_scope;
	}
	return false;
}*/

//=================================================================================================
//
//=================================================================================================
const iCScope* ParserContext::get_var_scope(const std::string& identifier)const
{
	iCScope* scope = current_scope;

	while(NULL != scope)
	{
		//simple search for now - replace with a map or with binary search
		for(std::vector<iCVariable*>::iterator i=scope->vars.begin();i!=scope->vars.end();i++)
		{
			if(0 == (*i)->name.compare(identifier))
			{
				return scope;
			}
			/*iCStringList& var_names = (*i)->var_names;
			for(iCStringList::iterator j=var_names.begin();j!=var_names.end();j++)
			{
				if(0 == (*j)->compare(identifier))
				{
					return scope;
				}
			}*/
		}

		//go up the scope tree
		scope = scope->prev_scope;
	}

	return NULL;
}

//=================================================================================================
//
//=================================================================================================
const iCScope* ParserContext::get_state_scope(const std::string& name)const
{
	iCScope* scope = current_scope;
	while(NULL != scope)
	{
		std::set<std::string>::iterator it = scope->states.find(name);
		if(scope->states.end() != it)
			return scope;

		//go up the scope tree
		scope = scope->prev_scope;
	}
	return NULL;
}

//=================================================================================================
//
//=================================================================================================
const iCScope* ParserContext::get_proc_scope(const std::string& name)const
{
	iCScope* scope = current_scope;
	while(NULL != scope)
	{
		std::set<std::string>::iterator it = scope->processes.find(name);
		if(scope->processes.end() != it)
			return scope;

		//go up the scope tree
		scope = scope->prev_scope;
	}
	return NULL;
}

//=================================================================================================
//
//=================================================================================================
void ParserContext::add_mcu_decl_to_scope( const std::string& name )
{
	root_scope->mcu_decls.insert(name);
}

//=================================================================================================
//
//=================================================================================================
const iCScope* ParserContext::get_mcu_decl_scope( const std::string& mcu_decl ) const
{
	iCScope* scope = current_scope;

	while(NULL != scope)
	{
		//Check if it's an mcu identifier (vector, register or bit) declared in this scope
		std::set<std::string>::iterator it = scope->mcu_decls.find(mcu_decl);
		if(scope->mcu_decls.end() != it)
			return scope;

		//go up the scope tree
		scope = scope->prev_scope;
	}

	return NULL;
}

//=================================================================================================
//
//=================================================================================================
void ParserContext::err_msg( const char* format, ... )
{
	had_errors = true;
	char buffer[ERR_MSG_BUFFER_SIZE];
	va_list args;
	va_start(args,format);
	vsprintf(buffer,format, args);
	va_end(args);
	std::cout<<file_name()<<":"<<line()/*<<":"<<column()*/<<": error: "<<buffer<<std::endl;
}

iCVariable* ParserContext::get_var( const std::string& identifier ) const
{
	iCScope* scope = current_scope;

	while(NULL != scope)
	{
		//simple search for now - replace with a map or with binary search
		for(std::vector<iCVariable*>::iterator i=scope->vars.begin();i!=scope->vars.end();i++)
		{
			if(0 == (*i)->name.compare(identifier))
			{
				return *i;
			}
		}

		//go up the scope tree
		scope = scope->prev_scope;
	}

	return NULL;
}