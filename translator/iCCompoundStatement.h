#pragma once

#include "iCNode.h"
class ParserContext;
class CodeGenContext;

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
};

/*
class iCFunctionBody : public iCCompoundStatement
{
public:
	iCFunctionBody(const ParserContext& context);
	virtual ~iCFunctionBody();
	virtual void gen_code(CodeGenContext& context);
};
*/