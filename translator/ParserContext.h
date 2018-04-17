#pragma once

#include "iCNode.h"
#include <stdio.h>
#include <stdarg.h>

class iCProgram;
class iCProcess;
class iCState;
class iCScope;
class iCVariable;
class iCFunction;

//=================================================================================================
//Parser context
//Used and updated by the parser
//=================================================================================================
class ParserContext
{
	const iCProgram* program;//does not own
	iCProcess* process;//does not own
	iCState* state;
	iCFunction* func;//does not own

	bool in_func;

	//Location info
	unsigned long line_num;
	unsigned long start_col;
	unsigned long end_col;
	
	std::string filename;

	iCScope* root_scope;
	iCScope* current_scope;

public:
	//var_declaration needs to know if we are inside function to decide whether
	//the vars should be made local or global 
	bool in_function()const{return NULL != func;}
	void set_func(iCFunction* func) {this->func = func;}
	iCFunction*  get_func() {return func;}

	std::set<iCNode*> second_pass_nodes; //does not own

	ParserContext();
	~ParserContext();

	void set_program(const iCProgram* program) {this->program = program;}
	void set_process(iCProcess* process) {this->process = process;}
	void set_state(iCState* state) {this->state = state;}
	
	const iCProgram* get_program() const {ICASSERT(NULL != program); return program;}
	const iCProcess* get_process() const {/*ICASSERT(NULL != process);*/ return process;}//removed assert because vars in functions are used outside processes
	iCProcess* modify_process() {return process;}
	const iCState* get_state() const {ICASSERT(NULL != state); return state;}
	iCState* modify_state() {return state;}
	
	unsigned long line() const {return line_num;}
	unsigned long column() const {return start_col;}

	void add_to_second_pass(iCNode* node)
	{
		second_pass_nodes.insert(node);
	}

	void rm_from_second_pass(iCNode* node)
	{
		std::set<iCNode*>::iterator n = second_pass_nodes.find(node);
		if(second_pass_nodes.end() != n)
			second_pass_nodes.erase(n);
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

	void err_msg(const char* format, ...);

	void open_scope(const std::string& name = "");
	void close_scope();
	void add_var_to_scope(iCVariable* decl);
	void add_state_to_scope(const std::string& name);
	void add_proc_to_scope(const std::string& name);
	void add_mcu_decl_to_scope(const std::string& name);
	void add_func_to_scope(const std::string& func);

	//bool check_scope(const std::string& identifier)const;
	const iCScope* get_var_scope(const std::string& identifier)const;
	const iCScope* get_mcu_decl_scope(const std::string& mcu_decl)const;
	const iCScope* get_func_scope(const std::string& func)const;
	const iCScope* get_state_scope(const std::string& name)const;
	const iCScope* get_proc_scope(const std::string& name)const;
	const iCScope* get_current_scope()const{return current_scope;}

	iCVariable* get_var(const std::string& identifier)const;

	void check_identifier_defined(const std::string& identifier);
};
