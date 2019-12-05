#pragma once

#include "iCNode.h"

class ParserContext;
class CodeGenContext;

//=================================================================================================
//Atomic block (critical section) statement
//=================================================================================================
class iCAtomicBlock : public iCStatement
{
	iCStatement* body; // owns
public:
	iCAtomicBlock(const ParserContext& context) : iCNode(context), body(NULL) {}
	virtual ~iCAtomicBlock();
	void set_body(iCStatement* body) { this->body = body; }
	virtual void gen_code(CodeGenContext& context);
	
	virtual ICNODE_TYPE node_type() { return ICATOMICBLOCK; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCAtomicBlock";}
#endif//DEBUG
};