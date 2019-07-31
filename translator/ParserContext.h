#pragma once

#include "iCNode.h"
#include <stdarg.h>

class iCProgram;
class iCProcess;
class iCState;
class iCScope;
class iCVariable;
class iCProcTypeParam;
class iCFunction;

//=================================================================================================
//Parser context
//Used and updated by the parser
//=================================================================================================
class ParserContext
{
	const iCProgram* program; //does not own
	iCProcType* proctype; //does not own
	iCProcess* process; //non-parameterized process, does not own
	iCState* state; //does not own
	iCFunction* func; //does not own
	iCScope* root_scope; //owns
	iCScope* current_scope; //does not own

	bool _in_isr;
	bool _in_timeout;

	//Location info
	unsigned long line_num;
	unsigned long start_col;
	unsigned long end_col;
	std::string filename;
public:
	//used for critical section placement decisions
	bool in_isr() const {return _in_isr && !_in_timeout;}
	void enter_isr() {_in_isr = true;}
	void leave_isr() {_in_isr = false; }
	void enter_timeout() {_in_timeout = true; }
	void leave_timeout() {_in_timeout = false; }

	//var_declaration needs to know if we are inside function to decide whether
	//the vars should be made local or global 
	bool in_function()const{return NULL != func;}
	void set_func(iCFunction* func) {this->func = func;}
	iCFunction* get_func() {return func;}

	std::set<iCNode*> second_pass_nodes; //does not own

	ParserContext();
	~ParserContext();

	void set_program(const iCProgram* program) { this->program = program; }
	void set_proctype(iCProcType* proctype) { this->proctype = proctype; }
	void set_process(iCProcess* process) {this->process = process;}
	void set_state(iCState* state) {this->state = state;}
	
	const iCProgram* get_program() const {ICASSERT(NULL != program); return program;}
	const iCProcType* get_proctype() const { return proctype; }
	iCProcType* modify_proctype() const { return proctype; }
	const iCProcess* get_process() const {/*ICASSERT(NULL != process);*/ return process;}//removed assert because vars in functions are used outside processes
	iCProcess* modify_process() {return process;}
	const iCState* get_state() const {ICASSERT(NULL != state); return state;}
	iCState* modify_state() {return state;}
	
	unsigned long line() const {return line_num;}
	unsigned long column() const {return start_col;}

	//Add to set of iCNode*s in order to call their second_pass() methods during the secondary (post-parsing) analysis
	void add_to_second_pass(iCNode* node)
	{
		second_pass_nodes.insert(node);
	}

	void inc_line_num() {line_num++; end_col = start_col = 1;}
	void inc_column(int shift){start_col = end_col; end_col += shift;}
	void dec_column(int shift){start_col = end_col; end_col -= shift;}
	void to_prev_column(){end_col = start_col;}
	void tab_column(int tab_size){start_col = end_col; end_col += tab_size - (end_col - 1) % tab_size;}
	void set_line(unsigned long new_line_number){line_num = new_line_number;}
	void reset_column() {start_col = end_col = 1;}
	void set_file(const std::string& filename) {this->filename = filename;}
	const std::string& file_name() const {return filename;}

	void err_msg(const char* format, ...)const;

	void open_scope(const std::string& name = "");
	void close_scope();
	void add_var_to_scope(iCVariable* decl);
	void add_proctype_param_to_scope(iCProcTypeParam* param);
	void add_state_to_scope(const std::string& name);
	void add_proc_to_scope(const std::string& name);
	void add_proctype_to_scope(const std::string& name);
	void add_mcu_decl_to_scope(const std::string& name);
	void add_func_to_scope(const std::string& func);

	const iCScope* get_proctype_param_scope(const std::string& identifier)const; //todo: remove?
	const iCScope* get_var_scope(const std::string& identifier)const;
	const iCScope* get_mcu_decl_scope(const std::string& mcu_decl)const;
	const iCScope* get_func_scope(const std::string& func)const;
	const iCScope* get_state_scope(const std::string& name)const;
	const iCScope* get_proc_scope(const std::string& name)const;
	const iCScope* get_proctype_scope(const std::string& name) const;
	const iCScope* get_current_scope()const{return current_scope;}

	iCVariable* get_var(const std::string& identifier)const;

	void check_identifier_defined(const std::string& identifier);
};
