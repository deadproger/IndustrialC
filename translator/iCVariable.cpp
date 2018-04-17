#include "iCVariable.h"
#include "iCScope.h"
#include "CodeGenContext.h"
#include "iCInitializer.h"

//=================================================================================================
//
//=================================================================================================
iCVariable::iCVariable( const std::string& name,
						const iCScope* scope,
						const ParserContext& context )
	:	name(name),
		scope(scope),
		iCNode(context),
		used_in_isr(false),
		decl_initializer(NULL)
{
	full_name = scope->name + "_" + name;
}

//=================================================================================================
//Code generator
//=================================================================================================
void iCVariable::gen_code( CodeGenContext& context )
{
#ifdef ICDEBUG_TRACE
	std::cout<<"iCVariable::gen_code " << name << "...";
	std::cout.flush();
#endif

	context.set_location(line_num, filename);
	context.indent();
	
	for(iCStringList::iterator i=type_specs.begin();i!=type_specs.end();i++)
		context.to_code_fmt("%s ", (*i).c_str());

	context.to_code_fmt(full_name.c_str());

	for(std::vector<iCExpression*>::iterator i=array_dimensions.begin();i!=array_dimensions.end();i++)
	{
		iCExpression* dimension = *i;
		context.to_code("[");
		if(NULL != dimension)
			dimension->gen_code(context);
		context.to_code("]");
	}

	if(NULL != decl_initializer)
	{
		context.to_code(" = ");
		decl_initializer->gen_code(context);
	}

	context.to_code_fmt(";");
#ifdef ICDEBUG_TRACE
	std::cout<<"done iCVariable\n";
	std::cout.flush();
#endif
}

//=================================================================================================
//
//=================================================================================================
iCVariable::~iCVariable()
{
	if(NULL != decl_initializer)
		delete decl_initializer;

	for(std::vector<iCExpression*>::iterator i=array_dimensions.begin();i!=array_dimensions.end();i++)
	{
		iCExpression* dimension = *i;
		if(NULL != dimension)
			delete dimension;
	}
}

//=================================================================================================
//
//=================================================================================================
void iCVariable::set_type_specs( const iCStringList& _type_specs )
{
	for(iCStringList::const_iterator i=_type_specs.begin();i!=_type_specs.end();i++)
	{
		type_specs.push_back(*i);
	}
}

//=================================================================================================
//Semantic check
//If var is used in ISR, not defined as constant and not qualified as volatile - gens a warning
//=================================================================================================
void iCVariable::second_pass()
{
	if(used_in_isr)
	{
		iCStringList::iterator findVolatile = std::find(type_specs.begin(), type_specs.end(), std::string("volatile"));
		iCStringList::iterator findConst = std::find(type_specs.begin(), type_specs.end(), std::string("const"));
		if(type_specs.end() == findVolatile && type_specs.end() == findConst)
			warning_msg("variable \"%s\" is used in ISR but not qualified as volatile", name.c_str());
	}
}

