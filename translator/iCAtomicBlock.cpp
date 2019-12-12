#include "iCAtomicBlock.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//
//=================================================================================================
iCAtomicBlock::~iCAtomicBlock()
{
	if(NULL != body)
		delete body;
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCAtomicBlock::gen_code( CodeGenContext& context )
{
	if(context.in_ISR())//atomic blocks inside ISR would cause nested interrupts
		return;

	//Add pre comment
	if(!pre_comment.empty() && context.retain_comments)
	{
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("%s", pre_comment.c_str());
	}

	context.set_location(line_num, filename);
	context.atomic_header();

	if(NULL != body)
		body->gen_code(context);

	context.atomic_footer();
}
