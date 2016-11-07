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
	std::vector<iCScope*> issues;
	std::vector<iCVariable*> vars; //does not own
	std::set<std::string> states;
	std::set<std::string> processes; //does not own
	std::set<std::string> mcu_decls;


	iCScope(const std::string& name);
	~iCScope();

	void delete_scope_tree(iCScope* root_scope);
};

