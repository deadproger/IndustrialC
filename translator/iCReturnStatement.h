#pragma once
#include "iCNode.h"

class ParserContext;
class CodeGenContext;

//=================================================================================================
//
//=================================================================================================
class iCReturnStatement : public iCStatement
{
	iCExpression* expr; // owns
public:
	iCReturnStatement(iCExpression* expr, const ParserContext& context);
	virtual ~iCReturnStatement();

	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCReturnStatement";}
#endif//DEBUG
};