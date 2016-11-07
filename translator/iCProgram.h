#pragma once

#include "iCNode.h"
#include "iCVariable.h"

class CodeGenContext;
class iCProcess;
class iCHyperprocess;
class ParserContext;

//=================================================================================================
//Program
//=================================================================================================
class iCProgram : public iCNode 
{
private:
	//std::string name;
	/*iCProgram(){};//no default constructor
	iCDeclarationList decl_list;
	iCProcessList proc_list;*/

	iCProcess* first_bkgrnd_process;

	iCHyperprocessMap hps;
	iCDeclarationList mcu_decls;
	iCProcessList procs; // does not own, auxiliary list for quick checks
	std::list<iCVariable*> var_list;

public:
	iCProgram(const ParserContext& context) : iCNode(context), first_bkgrnd_process(NULL){}
	//iCProgram(const iCDeclarationList& decl_list, const iCProcessList& proc_list);
	virtual ~iCProgram();

	void add_hyperprocess(iCHyperprocess* hp);
	void add_process(iCProcess* proc);
	void add_mcu_declaration(iCDeclaration* decl);
	void add_variable(iCVariable* var) 
	{	
		var_list.push_back(var);
	}
	bool hp_defined(const std::string& activator);

	const iCHyperprocessMap* get_hps() const {return &hps;} 
	const iCHyperprocess* get_hp(const std::string& hp_name) const;
	bool proc_defined(const std::string& proc_name) const;

	virtual void gen_code(CodeGenContext& context);
	virtual void second_pass();
};
