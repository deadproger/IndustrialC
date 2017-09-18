#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//Timeout
//=================================================================================================
class iCTimeout : public iCStatement
{
	//unsigned long period;
	iCExpression* period; // owns
	iCBlockItemsList body;
public:
	iCTimeout(iCExpression* period, const ParserContext& context) : period(period), iCNode(context) {}
	//iCTimeout(unsigned long period, const iCBlockItemsList& body, const ParserContext& context) : period(period), body(body) {}
	virtual ~iCTimeout();

	void set_items(const iCBlockItemsList& items) {body = items;}

	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCTimeout";}
#endif//DEBUG
};
