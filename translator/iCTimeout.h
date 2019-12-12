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
	iCTimeout(iCExpression* period, const ParserContext& context);
	//iCTimeout(const iCTimeout& timeout);
	virtual ~iCTimeout();
	void set_items(const iCBlockItemsList& items) {body = items;}
	virtual void gen_code(CodeGenContext& context);
	
	virtual ICNODE_TYPE node_type() { return ICTIMEOUT; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCTimeout";}
#endif//DEBUG
};
