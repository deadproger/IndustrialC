#pragma once

#include "iCNode.h"
#include "iCVariable.h"

class CodeGenContext;
class iCProcess;
class iCHyperprocess;
class ParserContext;
class iCFunction;

//=================================================================================================
//Program - root node in AST
//=================================================================================================
class iCProgram : public iCNode 
{
	iCProcess* first_bkgrnd_process;//process that is initially active
	iCHyperprocessMap hps;//map of all hyperprocesses
	iCDeclarationList mcu_decls;//vector/register/bit name definitions
	iCProcessList procs; // does not own, auxiliary list for quick checks
	std::list<iCVariable*> var_list;//list of defined variables
	std::list<iCFunction*> func_list;//list of defined functions

public:
	iCProgram(const ParserContext& context) : iCNode(context), first_bkgrnd_process(NULL){}
	virtual ~iCProgram();
	void add_hyperprocess(iCHyperprocess* hp);
	void add_process(iCProcess* proc);
	void add_mcu_declaration(iCDeclaration* decl);
	void add_variable(iCVariable* var) { var_list.push_back(var); }
	void add_function(iCFunction* func);
	bool hp_defined(const std::string& activator);
	const iCHyperprocessMap* get_hps() const {return &hps;} 
	const iCHyperprocess* get_hp(const std::string& hp_name) const;
	bool proc_defined(const std::string& proc_name) const;
	const iCProcess* find_proc(const std::string& proc_name)const;
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCProgram"}
#endif//DEBUG
};
