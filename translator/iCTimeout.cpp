#include "iCTimeout.h"
#include "CodeGenContext.h"
#include "iCProcess.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCTimeout::gen_code( CodeGenContext& context )
{
	ICASSERT(!context.in_ISR());

	iCProcess* proc = context.process;
	if (NULL == proc)
	{
		proc = context.proctype_instantiation;
	}

	bool need_atomic_block = proc->is_isr_driven() || proc->is_isr_referenced();

	//if process time can be modified from ISR, copy it atomically and use the copy
	if(need_atomic_block)
	{
		//atomically fetch the process's time
		context.atomic_header();
		context.to_code_fmt("%s = %s[%s].%s;",	C_COMMON_BKG_PROC_TIME,
												C_PROC_ARRAY_NAME,
												proc->name.c_str(),
												C_STATE_TIME_ATTR_NAME);
		context.atomic_footer();
	}

	//timeout header
	context.set_location(line_num, filename);
	context.indent();
	if(need_atomic_block)
		context.to_code_fmt("if((%s - %s) >= ",C_SYS_TIME_CUR_NAME, C_COMMON_BKG_PROC_TIME);
	else
		context.to_code_fmt("if((%s - %s[%s].%s) >= ",C_SYS_TIME_CUR_NAME, C_PROC_ARRAY_NAME, proc->name.c_str(), C_STATE_TIME_ATTR_NAME);
	
	period->gen_code(context);
	context.to_code_fmt(")");
	context.to_code_fmt("//Timeout\n");

	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	//push statement nodes in stack in reverse order
	for(iCBlockItemsList::iterator i=body.begin();i!=body.end();i++)
	{
		(*i)->gen_code(context);
	}

	//timeout footer
	context.indent_depth--;
	context.to_code_fmt("\n");
	context.indent();
	context.to_code_fmt("}");
}

//iCTimeout::iCTimeout(const iCTimeout& timeout)
//{
//	//iCNode fields
//	this->line_num = timeout.line_num;
//	this->col_num = timeout.col_num;
//	this->filename = timeout.filename;
//
//	this->period = timeout.period; //ptr
//	this->body;
//}

//=================================================================================================
//
//=================================================================================================
iCTimeout::~iCTimeout()
{
	for(iCBlockItemsList::iterator i=body.begin();i!=body.end();i++)
	{
		delete *i;
	}
	if(NULL != period)
		delete period;
}