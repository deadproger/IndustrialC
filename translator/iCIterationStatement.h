#pragma once

#include "iCNode.h"

class ParserContext;
class CodeGenContext;

class iCIterationStatement : public iCStatement
{
	iCStatement* body; // owns
	iCStatement* init; // owns
	iCStatement* condition; // owns
	iCExpression* increment; // owns
public:
	iCIterationStatement(iCStatement* init,
						 iCStatement* condition,
						 iCExpression* increment,
						 iCStatement* body,
						 const ParserContext& context);
	virtual ~iCIterationStatement();
	virtual void gen_code(CodeGenContext& context);
	
	virtual ICNODE_TYPE node_type() { return ICITERATIONSTATEMENT; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCIterationStatement";}
#endif//DEBUG
};