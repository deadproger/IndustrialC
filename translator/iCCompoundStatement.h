#pragma once

#include "iCNode.h"
class ParserContext;
class CodeGenContext;

class iCCompoundStatement : public iCStatement
{
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
};