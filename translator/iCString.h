#pragma once

#include "iCNode.h"

class ParserContext;
class CodeGenContext;

//=================================================================================================
//String constant
//=================================================================================================
class iCString : public iCExpression
{
private:
	std::string str;
public:
	iCString(const std::string& str, const ParserContext& context);
	~iCString(){}
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCString";}
#endif//DEBUG
};

