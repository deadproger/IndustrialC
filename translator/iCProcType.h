#pragma once

#include "iCNode.h"
#include "iCVariable.h"

class iCProcType : public iCDeclaration //todo: iCNode instead?
{
	iCStateList states; //owns
	iCVariablesList var_list; //list of defined variables (owns)
	iCProcTypeParamList param_list; //list of given parameters (owns)
public:
	std::string name;
	iCProcType(const std::string& name, const iCProcTypeParamList& param_list, const ParserContext& context);
	~iCProcType();
	void add_states(const iCStateList& states);
	bool has_state(const std::string& state_name) const;
	void add_variable(iCVariable* var) { var_list.push_back(var); }
	const iCStateList& get_states() const { return states; }
	const iCVariablesList& get_variables() const { return var_list; }
	const iCProcTypeParamList& get_params() const { return param_list; }
	virtual void gen_code(CodeGenContext& context); //todo: remove gen_code
};
