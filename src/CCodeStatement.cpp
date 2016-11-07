#include "CCodeStatement.h"
#include "CodeGenContext.h"
#include "CCode.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void CCodeStatement::gen_code(CodeGenContext& context)
{
	if(NULL != ccode_block)
	{
		context.set_location(line_num, filename);
		ccode_block->gen_code(context);
	}
}

//=================================================================================================
//
//=================================================================================================
CCodeStatement::~CCodeStatement()
{
	delete ccode_block;
}


