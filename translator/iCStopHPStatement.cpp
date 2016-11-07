#include "iCStopHPStatement.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCProgram.h"
#include "iCHyperprocess.h"

//=================================================================================================
//
//=================================================================================================
void iCStopHPStatement::second_pass()
{
	hp = program->get_hp(hp_name);
	if(NULL == hp)
	{
		err_msg("undefined hyperprocess %s", hp_name.c_str());
	}
	if(0 == hp_name.compare("background"))
	{
		err_msg("stop action not defined for background hyperprocess");
	}
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCStopHPStatement::gen_code( CodeGenContext& context )
{
	if(NULL == hp) 
		return;

	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("CLR_BIT(%s, %s);\n", hp->int_ctrl_register.c_str(), hp->int_ctrl_bit.c_str());
}

//=================================================================================================
//
//=================================================================================================
iCStopHPStatement::iCStopHPStatement( const std::string& hp_name, const ParserContext& context )
	:	hp_name(hp_name),
		hp(NULL),
		program(context.get_program()),
		iCNode(context)
{
	line_num = context.line();
}