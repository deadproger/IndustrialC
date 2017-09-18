#pragma once
#include "iCNode.h"
class CodeGenContext;

class iCInitializer : public iCExpression
{
private:
	 std::vector<iCExpression*> initializers;//some of these could be iCInitializer instances themselves
public:
	iCInitializer(const ParserContext& context)
		: iCNode(context)
		{}
	~iCInitializer();
	void add_initializer(iCExpression* initializer)
	{
		if(NULL != initializer)
			initializers.push_back(initializer);
	}
	virtual void gen_code(CodeGenContext& context);

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCInitializer";}
#endif//DEBUG
};