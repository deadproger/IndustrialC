#pragma once

#include "iCNode.h"

class CodeGenContext;
class ParserContext;
class iCTimeout;

//=================================================================================================
//State
//=================================================================================================
class iCState : public iCNode 
{
	iCTimeout* timeout;//owns if present
	bool isr_driven;
public:
	bool special; //if it's state which doesn't need prefix before name (START_STATE_NAME and STOP_START_NAME)
	std::string name;
	iCBlockItemsList block_items;

	iCState(const std::string& name, const ParserContext& context);
	//iCState(const iCState&);
	virtual ~iCState();
	void set_timeout(iCTimeout* timeout_statement)
	{
		if(NULL != timeout)
			err_msg("timeout redefinition in state %s", name.c_str());
		else
			timeout = timeout_statement;
	}

	virtual void gen_code(CodeGenContext& context);
	void set_items(const iCBlockItemsList& items) {block_items = items;}
	bool operator == (const iCState& state) {return this->name == state.name;}
	void set_isr_driven(){isr_driven = true;}
	bool has_timeout(){return NULL != timeout;}
	void gen_timeout_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCState name: " + name;}
#endif//DEBUG
};

