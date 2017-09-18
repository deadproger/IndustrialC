#pragma once

#include "iCNode.h"
class CodeGenContext;
class iCVariable;
class iCScope;

class iCVariableDeclaration : public iCDeclaration
{
	std::list<iCVariable*> vars;//owns
public:
	iCVariableDeclaration(const ParserContext& context);
	~iCVariableDeclaration();
	void set_vars(const std::list<iCVariable*> var_list)
	{
		vars = var_list;
	}
	virtual void gen_code(CodeGenContext& context);
#ifdef DEBUG
	virtual const std::string& identify() const { return "iCVariableDeclaration";}
#endif//DEBUG
};