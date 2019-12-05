#pragma once

#include "iCIdentifier.h"

//=================================================================================================
//MCU-specific declaration: interrupt vector, register or bit
//=================================================================================================
class iCMCUIdentifier : public iCIdentifier
{
public:
	iCMCUIdentifier(const std::string& name, const ParserContext& context);
	//virtual ~iCMCUIdentifier() {}
	virtual void gen_code(CodeGenContext& context);
	
	virtual ICNODE_TYPE node_type() { return ICMCUIDENTIFIER; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCMCUIdentifier";}
#endif//DEBUG
};
