#include "iCDouble.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCDouble::gen_code(CodeGenContext& context)
{
	context.set_location(line_num, filename);
	context.to_code_fmt("%f", val);
}

//=================================================================================================
//
//=================================================================================================
iCDouble::iCDouble( const std::string& str , const ParserContext& context) : iCNode(context)
{
	val = atof(str.c_str());
}