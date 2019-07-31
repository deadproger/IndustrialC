#pragma once

#include "iCNode.h"
class CodeGenContext;
class iCScope;

//=================================================================================================
//Identifier
//=================================================================================================
class iCIdentifier : public iCExpression
{
public:
	std::string name;
	const iCScope* scope; //doesn't own
	iCIdentifier(const std::string& name, const iCScope* scope, const ParserContext& context);
	virtual void gen_code(CodeGenContext& context);
	const std::string get_scoped_name();
};
