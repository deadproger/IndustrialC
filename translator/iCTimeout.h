#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//IndustrialC timeout statement
//=================================================================================================
class iCTimeout : public iCStatement
{
	iCExpression* period; // owns
	iCBlockItemsList body; // owns
public:
	iCTimeout(iCExpression* period, const ParserContext& context) : period(period), iCNode(context) {}
	//iCTimeout(const iCTimeout& timeout);
	virtual ~iCTimeout();
	void set_items(const iCBlockItemsList& items) {body = items;}
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCTimeout";}
#endif//DEBUG
};
