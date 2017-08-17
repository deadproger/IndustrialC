#include "iCFunction.h"
#include "iCScope.h"
#include "CodeGenContext.h"

iCFunction::iCFunction( const iCStringList& type_specs_,
						const std::string name,
						const iCScope* scope,
						const ParserContext& context )
	:	//type_specs(type_specs),
		name(name),
		scope(scope),
		iCNode(context),
		body(NULL)
{
	for(iCStringList::const_iterator i=type_specs_.begin();i!=type_specs_.end();i++)
	{
		type_specs.push_back(*i);
	}

	//functions don't need prefixes - they are all in global scope
	//this also eases using C functions
	full_name = /*scope->name + "_" + */name;
}

iCFunction::~iCFunction()
{
	if(NULL != body)
		delete body;
}

void iCFunction::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);
	context.indent();
	for(iCStringList::iterator i=type_specs.begin();i!=type_specs.end();i++)
	{
		context.to_code_fmt("%s ", (*i).c_str());
	}

	context.to_code_fmt(full_name.c_str());
	context.to_code("()");
	if(NULL != body)
	{
		context.to_code_fmt("\n");
		body->gen_code(context);
	}
	else
		context.to_code(";");
}