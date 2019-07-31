#include "ParserContext.h"
#include "iCScope.h"
#include "iCVariable.h"
#include "iCProcTypeParam.h"

extern bool had_errors;

//=================================================================================================
//
//=================================================================================================
ParserContext::ParserContext()
	:	program(NULL),
		proctype(NULL),
		process(NULL),
		state(NULL),
		line_num(1),
		end_col(1),
		start_col(1),
		root_scope(NULL),
		current_scope(NULL),
		func(NULL),
		_in_isr(false),
		_in_timeout(false)
{
	current_scope = root_scope = new iCScope("program");
}

//=================================================================================================
//
//=================================================================================================
ParserContext::~ParserContext()
{
	std::cout << "ParserContext entered destructor" << std::endl;
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

void ParserContext::add_proctype_param_to_scope(iCProcTypeParam* param)
{
	current_scope->proctype_params.push_back(param);
}

//=================================================================================================
//
//=================================================================================================
void ParserContext::add_state_to_scope(const std::string& name)
{
	current_scope->states.insert(name);
}

//=================================================================================================
//
//=================================================================================================
void ParserContext::add_proc_to_scope(const std::string& name)
{
	current_scope->processes.insert(name);
}

void ParserContext::add_proctype_to_scope(const std::string& name)
{
	current_scope->proctypes.insert(name);
}

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
			if(0 == (*i)->name.compare(identifier))
				return scope;

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

const iCScope* ParserContext::get_proctype_scope(const std::string& name) const
{
	iCScope* scope = current_scope;
	while (NULL != scope)
	{
		std::set<std::string>::iterator it = scope->proctypes.find(name);
		if (scope->proctypes.end() != it)
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
void ParserContext::add_func_to_scope( const std::string& name )
{
	root_scope->funcs.insert(name);
}

const iCScope* ParserContext::get_proctype_param_scope(const std::string& identifier) const
{
	iCScope* scope = current_scope;

	while (NULL != scope)
	{
		//todo: simple search for now - replace with a map or with binary search
		for (iCProcTypeParamList::iterator i = scope->proctype_params.begin(); i != scope->proctype_params.end(); i++)
			if (0 == (*i)->name.compare(identifier))
				return scope;

		//go up the scope tree
		scope = scope->prev_scope;
	}
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
//Find iCScope where function func is declared. If such iCScope doesn't exist then return NULL.
//=================================================================================================
const iCScope* ParserContext::get_func_scope( const std::string& func ) const
{
	iCScope* scope = current_scope;

	while(NULL != scope)
	{
		//Check if it's an function name declared in this scope
		std::set<std::string>::iterator it = scope->funcs.find(func);
		if(scope->funcs.end() != it)
			return scope;

		//go up the scope tree
		scope = scope->prev_scope;
	}

	return NULL;
}

//=================================================================================================
//
//=================================================================================================
void ParserContext::err_msg( const char* format, ... )const
{
	had_errors = true;
	char buffer[ERR_MSG_BUFFER_SIZE];
	va_list args;
	va_start(args,format);
	vsprintf(buffer,format, args);
	va_end(args);
	std::cout<<file_name()<<":"<<line()/*<<":"<<column()*/<<": error: "<<buffer<<std::endl;
}

//=================================================================================================
//
//=================================================================================================
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

//=================================================================================================
//Checks the identifier in argument against all separate scopes(namespaces) and gens an error 
//if found
//=================================================================================================
void ParserContext::check_identifier_defined( const std::string& identifier )
{
	const iCScope* proctype_param_scope = get_proctype_param_scope(identifier);
	const iCScope* var_scope = get_var_scope(identifier);
	const iCScope* mcu_decl_scope = get_mcu_decl_scope(identifier);									
	const iCScope* func_scope = get_func_scope(identifier);
	if(NULL != proctype_param_scope || NULL != var_scope || NULL != mcu_decl_scope || NULL != func_scope)
	{
		const iCScope* scope = (NULL != proctype_param_scope)?proctype_param_scope:
			((NULL != var_scope)?var_scope:((NULL != mcu_decl_scope)?mcu_decl_scope:func_scope));
		err_msg("symbol redefinition: %s already defined in %s",
			identifier.c_str(), scope->name.empty()?"this scope":scope->name.c_str());
	}
}