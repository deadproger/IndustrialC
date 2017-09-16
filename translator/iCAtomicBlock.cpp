#include "iCAtomicBlock.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

iCAtomicBlock::~iCAtomicBlock()
{
	if(NULL != body)
		delete body;
}

void iCAtomicBlock::gen_code( CodeGenContext& context )
{
	if(context.in_ISR())//atomic blocks inside ISR would cause nested interrupts
		return;

	context.set_location(line_num, filename);

	context.indent();
	context.to_code_fmt(C_ATOMIC_BLOCK_START);
	context.to_code_fmt("\n");
	
	if(NULL != body)
		body->gen_code(context);

	context.to_code_fmt("\n");
	context.indent();
	context.to_code_fmt(C_ATOMIC_BLOCK_END);
}
