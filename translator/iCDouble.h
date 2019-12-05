#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//FP constant
//=================================================================================================
class iCDouble : public iCExpression
{
public:
	double val;
	iCDouble(double val, const ParserContext& context) : val(val), iCNode(context) {}
	iCDouble(const std::string& str, const ParserContext& context);
	virtual void gen_code(CodeGenContext& context);
	virtual ICNODE_TYPE node_type() { return ICDOUBLE; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCDouble";}
#endif//DEBUG
};