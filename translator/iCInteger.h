#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//Integer constant
//=================================================================================================
class iCInteger : public iCExpression
{
public:
	std::string val;
	iCInteger(const std::string& str, const ParserContext& context);
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCInteger";}
#endif//DEBUG
};
