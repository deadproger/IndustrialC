#pragma once

#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//C-style type cast
//=================================================================================================
class iCCastExpression : public iCExpression
{
private:
	std::string type_name;
	iCExpression* expr; // owns
public:
	iCCastExpression(const std::string& type_name, iCExpression* expr, const ParserContext& context);
	~iCCastExpression();
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCCastExpression";}
#endif//DEBUG
};