#pragma once

#include "iCIdentifier.h"
#include "CodeGenContext.h"
#include "iCScope.h"

class iCIdentifierInProcType : public iCIdentifier
{
public:
	iCIdentifierInProcType(const std::string& name, const iCScope* scope, const ParserContext& context)
		: iCIdentifier(name, scope, context),
		  iCNode(context) {}
	void gen_code(CodeGenContext& context);
};
