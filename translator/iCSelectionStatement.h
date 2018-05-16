#pragma once

#include "iCNode.h"
class ParserContext;
class CodeGenContext;

//=================================================================================================
//C abstract selection statement
//=================================================================================================
class iCSelectionStatement : public iCStatement
{
public:
	iCStatement *body; // owns
	iCExpression *expr; // owns
	iCSelectionStatement( const ParserContext& context, iCStatement *body, iCExpression *expr );
	virtual ~iCSelectionStatement();

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCSelectionStatement";}
#endif//DEBUG
};

//=================================================================================================
//C "if/else" statement
//=================================================================================================
class iCIfElseStatement : public iCSelectionStatement
{
public:
	iCStatement *else_body; // owns
	iCIfElseStatement( const ParserContext& context, iCStatement *body, iCStatement *else_body, iCExpression *expr )
		: iCSelectionStatement(context, body, expr), iCNode(context), else_body(else_body) {}
	virtual ~iCIfElseStatement();
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCIfElseStatement";}
#endif//DEBUG
};

//=================================================================================================
//C switch statement
//=================================================================================================
class iCSwitchStatement : public iCSelectionStatement
{
public:
	iCSwitchStatement( const ParserContext& context, iCStatement *body, iCExpression *expr ) :	iCSelectionStatement (context, body, expr), iCNode(context) {}
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCSwitchStatement";}
#endif//DEBUG
};

//=================================================================================================
//C case statement
//=================================================================================================
class iCCaseStatement : public iCSelectionStatement
{
public:
	iCCaseStatement( const ParserContext& context, iCStatement *body, iCExpression *expr ) :	iCSelectionStatement (context, body, expr), iCNode(context)  {}
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCCaseStatement";}
#endif//DEBUG
};

//=================================================================================================
//C default statement
//=================================================================================================
class iCDefaultStatement : public iCStatement
{
	iCStatement *body; // owns
	
public:
	iCDefaultStatement( const ParserContext& context, iCStatement *body) 
		:	iCNode(context), body(body) {}
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCDefaultStatement";}
#endif//DEBUG
};