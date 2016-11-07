#pragma once

#include "iCNode.h"

class CodeGenContext;
class ParserContext;

//=================================================================================================
//State
//=================================================================================================
class iCState : public iCNode 
{
public:
	bool special;
	std::string name;
	iCBlockItemsList block_items;

	iCState(const std::string& name, const ParserContext& context);
	virtual ~iCState();

	virtual void gen_code(CodeGenContext& context);
	void set_items(const iCBlockItemsList& items) {block_items = items;}
	bool operator == (const iCState& state) {return this->name == state.name;}
};

