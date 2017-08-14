#pragma once

#include "iCNode.h"

class iCScope;
class iCCompoundStatement;

class iCFunction : public iCDeclaration
{
public:
	iCFunction(	const iCStringList& type_specs_,
				const std::string name,
				const iCScope* scope,
				const ParserContext& context);
	~iCFunction();

	iCStringList type_specs;//owns
	std::string name;
	std::string full_name;
	iCStatement *body;
	const iCScope* scope;//doesn't own, scope where it was defined

	virtual void gen_code(CodeGenContext& context);
};