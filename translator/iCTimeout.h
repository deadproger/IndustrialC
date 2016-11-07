#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//Timeout
//=================================================================================================
class iCTimeout : public iCStatement
{
	unsigned long period;
	iCBlockItemsList body;
public:
	iCTimeout(unsigned long period, const ParserContext& context) : period(period), iCNode(context) {}
	//iCTimeout(unsigned long period, const iCBlockItemsList& body, const ParserContext& context) : period(period), body(body) {}
	virtual ~iCTimeout();

	void set_items(const iCBlockItemsList& items) {body = items;}

	virtual void gen_code(CodeGenContext& context);
};
