#pragma once

#include "iCNode.h"
class ParserContext;
class CodeGenContext;

//=================================================================================================
//C "if/else" statement
//=================================================================================================
class iCSelectionStatement : public iCStatement
{
public:
	iCStatement *body; // owns
	iCStatement *else_body; // owns
	iCExpression *expr; // owns
	iCSelectionStatement( const ParserContext& context, iCStatement *body, iCStatement *else_body, iCExpression *expr );
	virtual ~iCSelectionStatement();
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCSelectionStatement";}
#endif//DEBUG
};