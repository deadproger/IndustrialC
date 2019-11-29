#include "iCCompoundStatement.h"
#include "CodeGenContext.h"

int iCCompoundStatement::wcet()
{
	
	int w = 0;
	for(iCBlockItemsList::iterator i=block_items.begin();i!=block_items.end();i++)
	{
		w += (*i)->wcet();
	}
	std::cout<<"iCCompoundStatement "<<w<<std::endl;//debaga
	return w;
}

//=================================================================================================
//
//=================================================================================================
iCCompoundStatement::iCCompoundStatement( const ParserContext& context )
	: iCNode(context)
{

}

//=================================================================================================
//
//=================================================================================================
void iCCompoundStatement::gen_code( CodeGenContext& context )
{
	context.set_location(line_num, filename);

	//header
	context.indent();
	context.to_code_fmt("{\n");
	context.indent_depth++;

	//body
	for(iCBlockItemsList::iterator i=block_items.begin();i!=block_items.end();i++)
		(*i)->gen_code(context);

	//footer
	context.indent_depth--;
	context.to_code_fmt("\n");
	context.indent();
	context.to_code_fmt("}");
}

//=================================================================================================
//
//=================================================================================================
iCCompoundStatement::~iCCompoundStatement()
{
	for(iCBlockItemsList::iterator i=block_items.begin();i!=block_items.end();i++)
		delete *i;
}
