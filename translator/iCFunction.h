#pragma once

#include "iCNode.h"

class iCScope;
class iCCompoundStatement;
class iCVariable;

//=================================================================================================
//C function definition
//=================================================================================================
class iCFunction : public iCDeclaration
{
public:
	iCStringList type_specs;//owns
	std::string name;
	std::string full_name;
	iCVariablesList params;//owns
	iCStatement *body; //owns
	const iCScope* scope;//doesn't own, scope where the func was defined

	iCFunction(	//const iCStringList& type_specs_,
				const std::string& name,
				const iCScope* scope,
				const ParserContext& context);
	~iCFunction();
	void set_type_specs(const iCStringList& type_specs_)
	{
		for(iCStringList::const_iterator i=type_specs_.begin();i!=type_specs_.end();i++)
			type_specs.push_back(*i);
	}
	void set_params(const iCVariablesList& params_list) { params = params_list; }
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCFunction name: " + name;}
#endif//DEBUG
};