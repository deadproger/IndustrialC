#pragma once

#include "iCNode.h"
class CodeGenContext;
class iCVariable;

class iCScope
{
	static unsigned long current_id;
	const unsigned long id;
public:
	std::string name;
	iCScope* prev_scope; //does not own
	std::vector<iCScope*> issues; //owns
	std::vector<iCVariable*> vars; //does not own
	iCProcTypeParamList proctype_params; //does not own
	std::set<std::string> states;
	std::set<std::string> processes;
	std::set<std::string> proctypes;
	std::set<std::string> mcu_decls;
	std::set<std::string> funcs;

	iCScope(const std::string& name);
	~iCScope();
	void delete_scope_tree(iCScope* root_scope);
};

