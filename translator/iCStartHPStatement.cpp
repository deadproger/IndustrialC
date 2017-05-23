#include "iCStartHPStatement.h"
#include "CodeGenContext.h"

#include "iCProgram.h"
#include "iCHyperprocess.h"
#include "ParserContext.h"

//=================================================================================================
//Semantic checks
//=================================================================================================
void iCStartHPStatement::second_pass()
{
	//retrieve the hyperprocess
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
void iCStartHPStatement::gen_code( CodeGenContext& context )
{
	if(NULL == hp) 
		return;

	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("SET_BIT(%s, %s);\n", hp->int_ctrl_register.c_str(), hp->int_ctrl_bit.c_str());
}

//=================================================================================================
//
//=================================================================================================
iCStartHPStatement::iCStartHPStatement( const std::string& hp_name, const ParserContext& context )
	:	hp_name(hp_name),
		program(context.get_program()),
		iCNode(context)
{
	line_num = context.line();
}