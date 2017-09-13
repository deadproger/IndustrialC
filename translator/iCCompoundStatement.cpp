#include "iCCompoundStatement.h"
#include "CodeGenContext.h"

iCCompoundStatement::iCCompoundStatement( const ParserContext& context )
	: iCNode(context)
{

}

void iCCompoundStatement::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);

	//header
	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	for(iCBlockItemsList::iterator i=block_items.begin();i!=block_items.end();i++)
	{
		(*i)->gen_code(context);
	}

	//footer
	context.indent_depth--;
	context.to_code_fmt("\n");
	context.indent();
	context.to_code_fmt("}");
}

iCCompoundStatement::~iCCompoundStatement()
{
	for(iCBlockItemsList::iterator i=block_items.begin();i!=block_items.end();i++)
		delete *i;
}

/*

void iCFunctionBody::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);
	for(iCBlockItemsList::iterator i=block_items.begin();i!=block_items.end();i++)
	{
		(*i)->gen_code(context);
	}
}

iCFunctionBody::iCFunctionBody( const ParserContext& context )
	: iCCompoundStatement(context){}

iCFunctionBody::~iCFunctionBody(){}

*/