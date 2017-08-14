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
	full_name = scope->name + "_" + name;
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
	context.to_code_fmt(";\n");
	if(NULL != body)
		body->gen_code(context);
	else
		context.to_code(";");
}