#pragma once

#include "iCNode.h"
#include "iCVariable.h"
#include "iCProcTypeInstantiation.h" //why without this parser.y says 'doesn't name a type?'

class CodeGenContext;
class iCProcess;
class iCProcType;
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
	iCProctypeMap proctypes; //map of all proctypes

	//defined instances (still unchecked on proctype existing) for statements like "start process <inst_name>"
	//todo: replace with instantiations in parser context and gencode context
	iCProctypeInstantiationMap proctype_instantiations;

	iCDeclarationList mcu_decls;//vector/register/bit name definitions
	iCProcessMap procs; // does not own, auxiliary list for quick checks
	iCVariablesList var_list;//list of defined variables
	std::list<iCFunction*> func_list;//list of defined functions

public:
	iCProgram(const ParserContext& context) : iCNode(context), first_bkgrnd_process(NULL){}
	virtual ~iCProgram();
	void add_hyperprocess(iCHyperprocess* hp);
	void add_proctype(iCProcType* proctype);
	void add_proctype_instantiation(iCProcTypeInstantiation* instantiation);
	void add_process(iCProcess* proc);
	void add_mcu_declaration(iCDeclaration* decl);
	void add_variable(iCVariable* var) { var_list.push_back(var); }
	void add_function(iCFunction* func);
	bool hp_defined(const std::string& activator);
	const iCHyperprocessMap* get_hps() const {return &hps;} 
	const iCHyperprocess* get_hp(const std::string& hp_name) const;
	bool proctype_defined(const std::string& activator);
	const iCProcType* find_proctype(const std::string& proctype_name) const;
	bool proctype_instance_defined(const std::string& instance_name) const;
	const iCProcTypeInstantiation* find_proctype_instance(const std::string& instance_name) const;
	bool proc_defined(const std::string& proctype_name) const;
	const iCProcess* find_proc(const std::string& proc_name)const;
	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass() {}

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCProgram"}
#endif//DEBUG
};
