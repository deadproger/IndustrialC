#include "iCAssignmentExpression.h"
#include "CodeGenContext.h"
#include "ParserContext.h"

//================================================================================================
//Code generator
//=================================================================================================
void iCAssignmentExpression::gen_code(CodeGenContext& context)
{	
	if(NULL == unary ||NULL == assgn) 
	{
		return;
	}

	context.set_location(line_num, filename);
	unary->gen_code(context);
	context.to_code_fmt(" %s ", op.c_str());
	assgn->gen_code(context);
}

//=================================================================================================
//
//=================================================================================================
iCAssignmentExpression::~iCAssignmentExpression()
{
	delete unary;
	delete assgn;
}

iCAssignmentExpression::iCAssignmentExpression( iCExpression* unary, const std::string& op, iCExpression* assgn, const ParserContext& context ) : unary(unary), op(op), assgn(assgn), iCNode(context){}