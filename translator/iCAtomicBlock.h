#pragma once

#include "iCNode.h"

class ParserContext;
class CodeGenContext;

class iCAtomicBlock : public iCStatement
{
private:
	iCStatement* body; // owns

public:
	iCAtomicBlock() : body(NULL) {}
	virtual ~iCAtomicBlock();

	void set_body(iCStatement* body) { this->body = body; }

	virtual void gen_code(CodeGenContext& context);
};