#include "iCVariable.h"
#include "iCScope.h"
#include "CodeGenContext.h"


iCVariable::iCVariable( const iCStringList& type_specs_, const std::string name, const iCScope* scope, const ParserContext& context )
	:	type_specs(type_specs),
		name(name),
		scope(scope),
		iCNode(context),
		used_in_isr(false)
{
	for(iCStringList::const_iterator i=type_specs_.begin();i!=type_specs_.end();i++)
	{
		type_specs.push_back(*i);
	}
	full_name = scope->name + "_" + name;
}

void iCVariable::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);
	context.indent();
	if(used_in_isr)
		context.to_code_fmt("volatile ");
	for(iCStringList::iterator i=type_specs.begin();i!=type_specs.end();i++)
	{
		context.to_code_fmt("%s ", (*i).c_str());
	}

	context.to_code_fmt(full_name.c_str());
	context.to_code_fmt(";\n");
}

iCVariable::~iCVariable()
{
	/*for(iCStringList::iterator i=type_specs.begin();i!=type_specs.end();i++)
		delete *i;*/
}