#pragma once

#include "iCNode.h"
#include "iCProcTypeInstantiation.h"
class iCProcess;
class iCState;

//Used by add_new_lines to decide: use actual newlines or line marker
const unsigned int NEW_LINES_THRESHOLD = 3;

//Size of buffer used by to_code_fmt
const unsigned int CODEGEN_BUFFER_SIZE = 1024;

//=================================================================================================
//Code generation context
//Used and updated by code generator methods
//All code generation is done via this class
//=================================================================================================
class CodeGenContext
{
	//Context
	std::string cur_filename;
	unsigned long cur_line_num;
	bool _in_ISR;
	
	//Stream to output resulting C-code to
	std::ostream& code;

	//Auxiliary
	void add_new_lines(int num = 1);//used to format code for better readability (uses actual \n's or line markers)
	void place_line_marker(unsigned long line_num, const std::string& file);
	void place_line_marker(unsigned long line_num);

public:
	CodeGenContext(std::ostream& code, const iCHyperprocessMap* hps)
		:	process(NULL), state(NULL), indent_depth(0), code(code), hps(hps),
			cur_line_num(0), _in_ISR(false), indent_enabled(true){}
	CodeGenContext(std::ostream& code)
		:	process(NULL), state(NULL), indent_depth(0), code(code), hps(NULL),
			cur_line_num(0), _in_ISR(false), indent_enabled(true){}

	std::string filename() const { return cur_filename; }
	unsigned long line() const { return cur_line_num; }

	//Adds newlines or line markers to sync C compiler output with industrialC lines and filenames
	void set_location(unsigned long line_num, const std::string& file);
	
	//Main code generation interface
	void to_code_fmt(const char* format, ...); //C style
	void to_code(const std::string& str); //Only for code without newlines
	void to_code_string(const std::string& str);//For code with newlines
	void atomic_header();//adds atomic block header to code
	void atomic_footer();//adds atomic block foother to code
	void disable_indentation(){indent_enabled = false;}
	void enable_indentation(){indent_enabled = true;}
	void indent(){ if(indent_enabled)code<<std::string(indent_depth, C_INDENT);}
	void flush(){code.flush();};

	//Used for atomic block placement decisions
	void enter_ISR(){_in_ISR = true;}
	void leave_ISR(){_in_ISR = false;}
	bool in_ISR()const{return _in_ISR;}

	//todo: unite process and instantiation
	iCProcess* process; //current process, does not own
	iCProcTypeInstantiation* proctype_instantiation; //current proctype instantiation, does not own
	iCState* state; //current state, does not own
	const iCHyperprocessMap* hps;//map of HPs - the program itself - does not own

	unsigned int indent_depth; //current indentation
	bool indent_enabled;
};
