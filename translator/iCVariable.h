#pragma once

#include "iCNode.h"

class iCScope;
class iCProcess;
class iCInitializer;

class iCVariable : public iCDeclaration//virtual iCNode
{
public:
	iCVariable(const std::string& name,
			   const iCScope* scope,
			   const ParserContext& context);
	~iCVariable();
	iCStringList type_specs;//owns
	std::string name;
	std::string full_name;
	const iCScope* scope;//doesn't own, scope where it was defined
	std::list<const iCProcess*> procs_use; // list of processes that reference this variable
	bool used_in_isr;//tells if var is reference from ISRs - used for automatic volatile

	iCInitializer* decl_initializer;//owns, can be NULL, initializers form a tree
	std::vector<iCExpression*> array_dimensions;//owns, dimensions can be NULL
	
	void set_initializer(iCInitializer* initializer){decl_initializer = initializer;}
	void add_dimension(iCExpression* dimension){array_dimensions.push_back(dimension);}
	void set_type_specs(const iCStringList& _type_specs);

	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCVariable full_name: " + full_name;}
#endif//DEBUG
};

