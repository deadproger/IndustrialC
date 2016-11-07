/*#include "iCVariableDeclaration.h"
#include "CodeGenContext.h"
#include "iCScope.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCVariableDeclaration::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);
	context.indent();
	for(iCStringList::iterator i=type_specs.begin();i!=type_specs.end();i++)
	{
		context.to_code("%s ", (*i)->c_str());
	}
	
	bool first = true;//crap code flag
	for(iCStringList::iterator i=var_names.begin();i!=var_names.end();i++)
	{
		//crap code to skip the comma before the first declarator
		if(!first)
			context.to_code(", ");
		else 
			first = false;

		//add scope prefix
		if(NULL != scope)
		if(!scope->name.empty())
			context.to_code("%s_", scope->name.c_str());
		context.to_code((*i)->c_str());
	}
	context.to_code(";\n");
}

//=================================================================================================
//
//=================================================================================================
iCVariableDeclaration::~iCVariableDeclaration()
{
	//type specs
	for(iCStringList::iterator i=type_specs.begin();i!=type_specs.end();i++)
		delete *i;

	//var names
	for(iCStringList::iterator i=var_names.begin();i!=var_names.end();i++)
	{
		delete *i;
	}
}
*/