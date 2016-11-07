#pragma once

#include "iCNode.h"
class iCProcess;
class iCState;

const unsigned int NEW_LINES_THRESHOLD = 0;
const unsigned int CODEGEN_BUFFER_SIZE = 1024;

//=================================================================================================
//Code generation context
//Used and updated by code generator methods
//=================================================================================================
class CodeGenContext
{
	std::string cur_filename;
	unsigned long cur_line_num;

	void add_new_lines(int num = 1)
	{
		cur_line_num += num;

		if(num > NEW_LINES_THRESHOLD)
			place_line_marker(cur_line_num);
		else
			for(int i=0;i<num;i++)
				code<<std::endl;
	}

	void place_line_marker(unsigned long line_num, const std::string& file)
	{
		cur_filename = file;
		cur_line_num = line_num;
		code<<std::endl<<"#line "<<line_num<<" \""<<file<<"\""<<std::endl;
	}

	void place_line_marker(unsigned long line_num)
	{
		cur_line_num = line_num;
		code<<std::endl<<"#line "<<line_num<<std::endl;
	}
	
public:

	std::string filename() const { return cur_filename; }
	unsigned long line() const { return cur_line_num; }

	void set_location(unsigned long line_num, const std::string& file)
	{
		if(file != cur_filename)
		{
			place_line_marker(line_num, file);
		}
		else
		{
			if(line_num < cur_line_num)
				place_line_marker(line_num);
			else
				add_new_lines(line_num - cur_line_num);
		}
	}
	
	void to_code_fmt(const char* format, ...)
	{
		char buffer[CODEGEN_BUFFER_SIZE];
		va_list args;
		va_start(args,format);
		vsprintf(buffer,format, args);
		va_end(args);

		//keep track of newlines
		std::string buf_str(buffer);
		cur_line_num += std::count(buf_str.begin(), buf_str.end(), '\n');

		code<<buffer;
	}
	void to_code(const std::string& str)
	{
		code<<str;
	}

	CodeGenContext(std::ostream& code, const iCHyperprocessMap* hps)
		:	process(NULL),
			state(NULL),
			indent_depth(0),
			code(code),
			hps(hps),
			cur_line_num(0){}
	CodeGenContext(std::ostream& code)
		:	process(NULL),
			state(NULL),
			indent_depth(0),
			code(code),
			hps(NULL),
			cur_line_num(0){}

	//CodeGenContext does not own pointed objects
	iCProcess* process; //current process
	iCState* state; //current state
	const iCHyperprocessMap* hps;

	unsigned int indent_depth; //current indentation
	std::ostream& code;

	void indent(){ code<<std::string(indent_depth, C_INDENT);}
};