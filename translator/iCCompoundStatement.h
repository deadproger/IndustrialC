#pragma once

#include "iCNode.h"
class ParserContext;
class CodeGenContext;

//=================================================================================================
//C compound statement - something enclosed in braces
//=================================================================================================
class iCCompoundStatement : public iCStatement
{
protected:
	iCBlockItemsList block_items;
public:
	iCCompoundStatement(const ParserContext& context);
	virtual ~iCCompoundStatement();
	void set_items(const iCBlockItemsList& items)
	{
		block_items = items;
	}

	virtual void gen_code(CodeGenContext& context);
	virtual bool is_compound() {return true;}
	iCBlockItemsList& get_block_items(){return block_items;}//a crutch


#ifdef DEBUG
	virtual const std::string& identify() const { return "iCCompoundStatement";}
#endif//DEBUG
};
