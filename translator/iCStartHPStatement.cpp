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
	//Check the hyperprocess's been defined somewhere in the program
	hp = program->get_hp(hp_name);//retrieve the hyperprocess
	if(NULL == hp)
	{
		err_msg("undefined hyperprocess %s", hp_name.c_str());
	}

	//check it's not background hp
	if(0 == hp_name.compare("background"))
	{
		err_msg("start action not defined for background hyperprocess");
	}
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCStartHPStatement::gen_code( CodeGenContext& context )
{
	if(NULL == hp) 
		return;

	//Add pre comment
	if(!pre_comment.empty() && context.retain_comments)
	{
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("%s", pre_comment.c_str());
	}
	
	//add atomic block if in background loop
	if(!context.in_ISR())
		context.atomic_header();

	//context.to_code_fmt("\n");
	context.set_location(line_num, filename);
	context.indent();
	context.to_code_fmt("SET_BIT(%s, %s);", hp->int_ctrl_register.c_str(), hp->int_ctrl_bit.c_str());

	//atomic block footer
	if(!context.in_ISR())
		context.atomic_footer();
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
	
	//appropriate the currently pending pre comment
	pre_comment = const_cast<ParserContext&>(context).grab_pre_comment();
}
