#include "iCFunction.h"
#include "iCScope.h"
#include "CodeGenContext.h"
#include "iCVariable.h"

iCFunction::iCFunction( //const iCStringList& type_specs_,
						const std::string& name,
						const iCScope* scope,
						const ParserContext& context )
	:	//type_specs(type_specs),
		name(name),
		scope(scope),
		iCNode(context),
		body(NULL)
{

	//functions don't need prefixes - they are all in global scope
	//this also eases using C functions
	full_name = /*scope->name + "_" + */name;
}

iCFunction::~iCFunction()
{
	for(std::list<iCVariable*>::iterator i=params.begin();i!=params.end();i++)
	{
		delete *i;
	}
	if(NULL != body)
		delete body;
}

void iCFunction::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);
	context.indent();

	//decl specs and name
	for(iCStringList::iterator i=type_specs.begin();i!=type_specs.end();i++)
	{
		context.to_code_fmt("%s ", (*i).c_str());
	}
	context.to_code_fmt(full_name.c_str());

	//params
	context.to_code("(");
	for(std::list<iCVariable*>::iterator i=params.begin();i!=params.end();i++)
	{
		//(*i)->gen_code(context);

		if(params.begin() != i)
			context.to_code(", ");

		//Special code generator for function parameter
		//no indentation, no initializer, semicolon or newline at the end
		//Should probably do this some other way..
		iCVariable* var = *i;
		context.set_location(var->line_num, var->filename);
		for(iCStringList::iterator i=var->type_specs.begin();i!=var->type_specs.end();i++)
		{
			context.to_code_fmt("%s ", (*i).c_str());
		}
		context.to_code_fmt(var->full_name.c_str());
		for(std::vector<iCExpression*>::iterator i=var->array_dimensions.begin();i!=var->array_dimensions.end();i++)
		{
			iCExpression* dimension = *i;
			context.to_code("[");
			if(NULL != dimension)
				dimension->gen_code(context);
			context.to_code("]");
		}
	}
	context.to_code(")");

	//body
	if(NULL != body)
	{
		context.to_code_fmt("\n");
		body->gen_code(context);
	}
	else
		context.to_code(";");
}