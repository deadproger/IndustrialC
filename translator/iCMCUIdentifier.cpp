#include "iCMCUIdentifier.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//
//=================================================================================================
iCMCUIdentifier::iCMCUIdentifier( const std::string& name, const ParserContext& context )
	: iCIdentifier(name, NULL, context),
	  iCNode(context)
{

}

//=================================================================================================
//
//=================================================================================================
void iCMCUIdentifier::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);
	context.to_code_fmt(name.c_str());
}