#pragma once

#include "iCNode.h"
class iCProcess;
class iCState;

const unsigned int NEW_LINES_THRESHOLD = 3;
const unsigned int CODEGEN_BUFFER_SIZE = 1024;

//=================================================================================================
//Code generation context
//Used and updated by code generator methods
//=================================================================================================
class CodeGenContext
{
	std::string cur_filename;
	unsigned long cur_line_num;

	void add_new_lines(int num = 1);
	void place_line_marker(unsigned long line_num, const std::string& file);
	void place_line_marker(unsigned long line_num);

	bool _in_ISR;
	
public:

	std::string filename() const { return cur_filename; }
	unsigned long line() const { return cur_line_num; }

	//Adds newlines or line markers to sync C compiler output with industrialC lines and filenames
	void set_location(unsigned long line_num, const std::string& file);
	
	void to_code_fmt(const char* format, ...);
	void to_code(const std::string& str);
	void to_code_string(const std::string& str);

	CodeGenContext(std::ostream& code, const iCHyperprocessMap* hps)
		:	process(NULL),
			state(NULL),
			indent_depth(0),
			code(code),
			hps(hps),
			cur_line_num(0),
			_in_ISR(false),
			indent_enabled(true){}
	CodeGenContext(std::ostream& code)
		:	process(NULL),
			state(NULL),
			indent_depth(0),
			code(code),
			hps(NULL),
			cur_line_num(0),
			_in_ISR(false),
			indent_enabled(true){}

	//CodeGenContext does not own these pointed objects
	iCProcess* process; //current process
	iCState* state; //current state
	const iCHyperprocessMap* hps;

	unsigned int indent_depth; //current indentation
	bool indent_enabled;

	void disable_indentation(){indent_enabled = false;}
	void enable_indentation(){indent_enabled = true;}

	std::ostream& code;

	void indent(){ if(indent_enabled)code<<std::string(indent_depth, C_INDENT);}

	void enter_ISR(){_in_ISR = true;}
	void leave_ISR(){_in_ISR = false;}
	bool in_ISR()const{return _in_ISR;}
};