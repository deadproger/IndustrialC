#include "iCString.h"
#include "ParserContext.h"
#include "CodeGenContext.h"

void iCString::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);
	context.to_code_string(str.c_str());
}

iCString::iCString( const std::string& str, const ParserContext& context)
	:	str(str), iCNode(context)
{

}
