#pragma once

#include "iCNode.h"
class CodeGenContext;

class iCFunctionCall : public iCExpression
{
private:
	std::vector<iCExpression*> args;
	std::string func_name;
public:
	iCFunctionCall(	const std::string& func_name,
					const ParserContext& context) 
		:	func_name(func_name),
			iCNode(context){}
	~iCFunctionCall();
	void set_args(const std::vector<iCExpression*>& arguments)
	{
		args = arguments;
	}
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCFunctionCall";}
#endif//DEBUG
};