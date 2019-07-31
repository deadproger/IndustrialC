#include "iCIdentifier.h"
#include "CodeGenContext.h"
#include "iCScope.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCIdentifier::gen_code(CodeGenContext& context)
{
	context.set_location(line_num, filename);

	//todo: use get_scoped_name method
	if(NULL != scope)
	if(!scope->name.empty())
	{
		context.to_code_fmt("%s_", scope->name.c_str());
	}
	context.to_code_fmt(name.c_str());
}

//=================================================================================================
//Returns identifier name with scope prefix
//=================================================================================================
const std::string iCIdentifier::get_scoped_name()
{
	if(NULL != scope)
		if(!scope->name.empty())
			return scope->name+"_"+name;
	return name;
}

//=================================================================================================
//
//=================================================================================================
iCIdentifier::iCIdentifier( const std::string& name, const iCScope* scope, const ParserContext& context ) 
: name(name), scope(scope), iCNode(context) 
{

}