#include "iCVariableDeclaration.h"
#include "iCVariable.h"
#include "CodeGenContext.h"
#include "iCScope.h"
#include "ParserContext.h"

//=================================================================================================
//Code generator
//=================================================================================================
void iCVariableDeclaration::gen_code( CodeGenContext& context )
{
	for(iCVariablesList::iterator i=vars.begin();i!=vars.end();i++)
	{
		if(!pre_comment.empty() && context.retain_comments)
		{
			//Add pre comment
			context.to_code_fmt("%s", pre_comment.c_str());
		}
		
		iCVariable* var = *i;
		if(NULL != var)
			var->gen_code(context);
	}
}

//=================================================================================================
//
//=================================================================================================
iCVariableDeclaration::iCVariableDeclaration(const ParserContext& context)
	: iCNode(context)
{
	//appropriate the currently pending pre comment
	pre_comment = const_cast<ParserContext&>(context).grab_pre_comment();
}

//=================================================================================================
//
//=================================================================================================
iCVariableDeclaration::~iCVariableDeclaration()
{
#ifdef ICDEBUG_TRACE
	std::cout<<"iCVariableDeclaration::gen_code " << "...";
	std::cout.flush();
#endif

	for(std::list<iCVariable*>::iterator i=vars.begin();i!=vars.end();i++)
		delete *i;

#ifdef ICDEBUG_TRACE
	std::cout<<"done iCVariableDeclaration\n";
	std::cout.flush();
#endif
}