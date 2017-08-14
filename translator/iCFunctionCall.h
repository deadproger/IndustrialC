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
	/*void add_argument(iCAssignmentExpression* arg)
	{
		args.push_back(arg);
	}*/
	virtual void gen_code(CodeGenContext& context);
};