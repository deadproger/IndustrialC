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
		//Add pre comment
		if(!pre_comment.empty() && context.retain_comments)
		{
			context.to_code_fmt("\n");
			context.indent();
			context.to_code_fmt("%s", pre_comment.c_str());
		}
		
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


