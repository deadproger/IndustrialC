#include "CCode.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCScope.h"
#include <sstream>
#include "iCIdentifier.h"
#include "iCVariable.h"
#include "iCProcess.h"

//all the characters allowed in variable names
//used for iC variables extraction
const std::string ID_CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

//=================================================================================================
//Code generator
//=================================================================================================
void CCodeLine::gen_code(CodeGenContext& context)
{
	context.to_code(text);
}

//=================================================================================================
//Code generator
//=================================================================================================
void CCode::gen_code(CodeGenContext& context)
{
	context.set_location(line_num, filename);

	for(CCodeLinesList::iterator i=lines.begin();i!=lines.end();i++)
	{
		context.indent();
		(*i)->gen_code(context);
		context.to_code_fmt("\n");
	}
}

//=================================================================================================
//C code line Constructor
//Replaces iC variables with their proper names in resulting C code
//=================================================================================================
CCodeLine::CCodeLine( const std::string& text, ParserContext& parser_context ) : iCNode(parser_context)
{
	this->text = text;
	
	//trim indentation
	size_t trim = this->text.find_first_not_of(" \t");
	this->text.erase(0, trim);

	//find escaped iC variables and replace them with their code generated names
	std::ostringstream text_stream;
	size_t pos = 0;
	size_t last_pos = 0;
	while((pos = this->text.find_first_of("$", last_pos)) != std::string::npos)
	{
		text_stream<<this->text.substr(last_pos, pos - last_pos); //dump clean substring to the stream
		pos++; //skip the dollar

		//extract variable name
		size_t end_pos = this->text.find_first_not_of(ID_CHARACTERS, pos);
		std::string var_name = this->text.substr(pos, end_pos - pos);

		//look up the var name in current scope 
		iCVariable* var = parser_context.get_var(var_name);
		if(NULL == var) // variable not defined in this scope
		{
			err_msg("undefined iC variable %s in c code section", var_name.c_str());
			text_stream<<var_name;//dump the var name anyway
		}
		else
		{
			//codegen the variable into the stream
			text_stream<<var->full_name;
			
			//Mark var as referenced in ISR - used for volatile checks
			const iCProcess* var_proc = parser_context.get_process();
			if(NULL != var_proc)
			{
				if(0 != var_proc->activator.compare("background"))
				{
					var->used_in_isr = true;
					parser_context.add_to_second_pass(var);
				}
			}
		}
		last_pos = pos = end_pos;
	}

	//dump the rest of the code
	if(last_pos != std::string::npos)
		text_stream<<this->text.substr(last_pos, this->text.length() - last_pos);//dump clean substring to the stream

	this->text = text_stream.str();
}

//=================================================================================================
//Default constructor
//=================================================================================================
CCode::CCode(  ParserContext& parser_context ) : scope(parser_context.get_current_scope()), iCNode(parser_context)
{

}

//=================================================================================================
//Destructor
//=================================================================================================
CCode::~CCode()
{
	for(CCodeLinesList::iterator i=lines.begin();i!=lines.end();i++)
		delete *i;
}

