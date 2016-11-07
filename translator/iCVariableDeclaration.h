#pragma once

/*
#include "iCNode.h"
class CodeGenContext;
class iCScope;

class iCVariableDeclaration : public iCDeclaration
{

public:
	iCStringList type_specs;
	iCStringList var_names;
	const iCScope* scope;

	iCVariableDeclaration(const iCStringList& type_specs, const iCStringList& var_names, const iCScope* scope, const ParserContext& context)
		:	type_specs(type_specs),
			var_names(var_names),
			scope(scope),
			iCNode(context){}
	~iCVariableDeclaration();

	virtual void gen_code(CodeGenContext& context);
};*/