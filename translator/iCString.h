#pragma once

#include "iCNode.h"

class ParserContext;
class CodeGenContext;


class iCString : public iCExpression
{
private:
	std::string str;
public:
	iCString(const std::string& str, const ParserContext& context);
	~iCString(){}

	virtual void gen_code(CodeGenContext& context);
};

