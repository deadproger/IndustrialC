#include "iCLogicConst.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//
//=================================================================================================
iCLogicConst::iCLogicConst( bool val, const ParserContext& context )
	:	val(val),
		iCNode(context)
{

}

//=================================================================================================
//
//=================================================================================================
void iCLogicConst::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);
	context.to_code(val?"true":"false");
}