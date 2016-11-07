#pragma once

#include "iCIdentifier.h"

class iCMCUIdentifier : public iCIdentifier
{
public:
	iCMCUIdentifier(const std::string& name, const ParserContext& context);
	virtual void gen_code(CodeGenContext& context);
};
