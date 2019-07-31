#pragma once

#include "iCIdentifier.h"
#include "CodeGenContext.h"
#include "iCScope.h"

class iCProcTypeParam : public iCIdentifier
{
public:
	std::string original_value = ""; //original argument value (used for processes in params)
	std::string scoped_value = ""; //argument value with scope prefix (used for variables in params)
	iCProcTypeParam(const std::string& name, const iCScope* scope, const ParserContext& context)
		: iCIdentifier(name, scope, context),
		iCNode(context) {}
	//virtual ~iCProcTypeParam() {}
	void gen_code(CodeGenContext& context);
};
