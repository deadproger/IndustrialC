#pragma once

#include "iCNode.h"
class CCode;
class CodeGenContext;

//=================================================================================================
//C Code Statement
//=================================================================================================
class CCodeStatement : public iCStatement
{
public:
	CCode* ccode_block;
	CCodeStatement(CCode* ccode, const ParserContext& context) : ccode_block(ccode), iCNode(context) {}
	virtual ~CCodeStatement();
	virtual void gen_code(CodeGenContext& context);
	
	virtual ICNODE_TYPE node_type() { return CCODESTATEMENT; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "CCodeStatement";}
#endif//DEBUG
};