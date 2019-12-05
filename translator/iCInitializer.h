#pragma once
#include "iCNode.h"
class CodeGenContext;

//=================================================================================================
//iCInitializer
//Represents var initializers, like in "int a = b + c - d*2" - everything after '=' is the initializer
//as well as array initializers, like " = {1,2,3,4}"
//and multidimensional array initializers, like " = {{1,2}, {3,4}}"
//In general case initializers form a tree
//=================================================================================================
class iCInitializer : public iCExpression
{
	std::vector<iCExpression*> initializers;//sub-initializers(issues), some of these could be iCInitializer instances themselves
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

	virtual ICNODE_TYPE node_type() { return ICINITIALIZER; }

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCInitializer";}
#endif//DEBUG
};