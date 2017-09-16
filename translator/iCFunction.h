#pragma once

#include "iCNode.h"

class iCScope;
class iCCompoundStatement;
class iCVariable;

class iCFunction : public iCDeclaration
{
public:
	iCFunction(	//const iCStringList& type_specs_,
				const std::string& name,
				const iCScope* scope,
				const ParserContext& context);
	~iCFunction();

	iCStringList type_specs;//owns
	std::string name;
	std::string full_name;
	std::list<iCVariable*> params;//owns
	//std::list<iCVariable*> local_vars;//owns
	iCStatement *body;
	const iCScope* scope;//doesn't own, scope where it was defined

	void set_type_specs(const iCStringList& type_specs_)
	{
		for(iCStringList::const_iterator i=type_specs_.begin();i!=type_specs_.end();i++)
		{
			type_specs.push_back(*i);
		}
	}

	void set_params(const std::list<iCVariable*>& params_list)
	{
		params = params_list;
	}

	/*
	void add_variable(iCVariable* var) 
	{	
		local_vars.push_back(var);
	}
	*/

	virtual void gen_code(CodeGenContext& context);
};