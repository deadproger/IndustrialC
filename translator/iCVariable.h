#pragma once

#include "iCNode.h"

class iCScope;
class iCProcess;

class iCVariable : public iCDeclaration//virtual iCNode
{
public:
	iCVariable(const iCStringList& type_specs, const std::string name, const iCScope* scope, const ParserContext& context);
	~iCVariable();
	iCStringList type_specs;//owns
	std::string name;
	std::string full_name;
	const iCScope* scope;//doesn't own, scope where it was defined
	std::list<const iCProcess*> procs_use; // list of processes that reference this variable
	bool used_in_isr;
	
	virtual void gen_code(CodeGenContext& context);
};