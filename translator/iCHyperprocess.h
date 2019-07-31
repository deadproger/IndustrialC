#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;
class iCProcess;

//=================================================================================================
//Hyperprocess
//=================================================================================================
class iCHyperprocess : public iCNode, public iCProgramItem
{
public:
	std::string int_vector;
	std::string int_ctrl_register;
	std::string int_ctrl_bit;
	unsigned long line_num;
	std::string activator;
	iCProcessMap procs;

	iCHyperprocess(const std::string& activator, const ParserContext& context);
	virtual ~iCHyperprocess();
	virtual void gen_code(CodeGenContext& context);
	void gen_timeout_code(CodeGenContext& context);
	void add_proc(iCProcess* proc);
	void set_params(const std::string& int_vec,
					const std::string& int_reg,
					const std::string& int_bit){int_vector = int_vec; int_ctrl_register = int_reg; int_ctrl_bit = int_bit;}

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCHyperprocess activator: " + activator;}
#endif//DEBUG
};