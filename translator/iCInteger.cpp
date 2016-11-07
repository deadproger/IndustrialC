#include "iCInteger.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCInteger::gen_code(CodeGenContext& context)
{
	context.set_location(line_num, filename);
	//context.to_code_fmt("%d", val);
	context.to_code(val);
}

iCInteger::iCInteger( const std::string& str, const ParserContext& context )
	:	val(str),
		iCNode(context)
{
	/*
	const char* c_str = str.c_str();
	//check if its dec, hex or bin
	if('0' == str[0])
	{
		if('x' == str[1])
		{
			val = strtol(c_str, NULL, 16);
		}
		else if('b' == str[1])
		{
			val = strtol(c_str+2, NULL, 2);
		}
	}
	else
	{
		val = strtol(c_str, NULL, 10);
	}
	*/
}