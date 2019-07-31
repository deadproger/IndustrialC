#include "iCFunction.h"
#include "iCScope.h"
#include "CodeGenContext.h"
#include "iCVariable.h"
#include "iCCompoundStatement.h"

iCFunction::iCFunction( const std::string& name,
						const iCScope* scope,
						const ParserContext& context )
	:	name(name),
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
	for(iCVariablesList::iterator i=params.begin();i!=params.end();i++)
	{
		delete *i;
	}
	
	if(NULL != body)
		delete body;
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCFunction::gen_code( CodeGenContext& context )
{
#ifdef ICDEBUG_TRACE
	std::cout<<"iCFunction::gen_code " << name << "...";
	std::cout.flush();
#endif

	context.to_code_fmt("\n");
	context.to_code_fmt("%s\n", C_COMMENT_FRAME);
	context.to_code_fmt("//Function: %s\n", name.c_str());
	context.to_code_fmt("%s\n", C_COMMENT_FRAME);
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
	for(iCVariablesList::iterator i=params.begin();i!=params.end();i++)
	{
		if(params.begin() != i)
			context.to_code(", ");

		//Special code generator for function parameter
		//no indentation, no initializer, semicolon or newline at the end
		iCVariable* var = *i;
		context.set_location(var->line_num, var->filename);
		for(iCStringList::iterator j=var->type_specs.begin();j!=var->type_specs.end();j++)
		{
			context.to_code_fmt("%s ", (*j).c_str());
		}
		context.to_code_fmt(var->full_name.c_str());
		for(std::vector<iCExpression*>::iterator j=var->array_dimensions.begin();j!=var->array_dimensions.end();j++)
		{
			iCExpression* dimension = *j;
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
		context.set_location(line_num, filename);
		context.indent();
		context.to_code_fmt("{\n");
		context.indent_depth++;

		iCBlockItemsList& items_list = reinterpret_cast<iCCompoundStatement*>(body)->get_block_items();//a crutch
		for(iCBlockItemsList::iterator i=items_list.begin();i!=items_list.end();i++)
			(*i)->gen_code(context);

		context.indent_depth--;
		context.to_code_fmt("\n");
		context.indent();
		context.to_code_fmt("}");
	}
	else
		context.to_code(";");

#ifdef ICDEBUG_TRACE
	std::cout<<"done iCFunction\n";
	std::cout.flush();
#endif
}