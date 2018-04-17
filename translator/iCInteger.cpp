#include "iCInteger.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCInteger::gen_code(CodeGenContext& context)
{
	context.set_location(line_num, filename);
	context.to_code(val);
}

//=================================================================================================
//
//=================================================================================================
iCInteger::iCInteger( const std::string& str, const ParserContext& context )
	:	val(str),
		iCNode(context)
{

}

