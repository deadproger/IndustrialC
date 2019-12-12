#include "CodeGenContext.h"
#include <sstream>

bool gen_line_markers = true;

const std::string WHITESPACE = " \t";

//Trims leading spaces and tabs in a string
std::string ltrim(const std::string& s) {
	size_t start = s.find_first_not_of (WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

//=================================================================================================
//Prints a precomment line-by-line with proper indentation, sets location for the following node
//=================================================================================================
void CodeGenContext::print_comment(const std::string& cmt, unsigned long line_num, const std::string& file)
{
	//Split the cmt string into a list of lines, trim the indentation
	std::stringstream ss(cmt);
	std::list<std::string> cmt_lines;
	std::string cmt_line;
	unsigned int cmt_num_lines = 0;
	while(std::getline(ss, cmt_line, '\n'))
	{
		cmt_line = ltrim(cmt_line);//trim indentation
		cmt_lines.push_back(cmt_line);
		cmt_num_lines++;
	}
	
	//Set location so that once we output the comments, the subsequent node is located properly
	to_code_fmt("\n");
	set_location(line_num - cmt_num_lines, file );
	
	//Output the comment batch line-by-line, adding proper indentation
	for(std::list<std::string>::iterator i=cmt_lines.begin();i!=cmt_lines.end();i++)
	{
		indent();
		to_code_fmt("%s\n", i->c_str());
	}
}

//=================================================================================================
//Adds num newlines to the code
//If the lines are too many (num > NEW_LINES_THRESHOLD) - use line marker instead to avoid big gaps
//=================================================================================================
void CodeGenContext::add_new_lines( int num /*= 1*/ )
{
	cur_line_num += num;

	if(gen_line_markers)
	{
		if(num > NEW_LINES_THRESHOLD)
			place_line_marker(cur_line_num);
		else
			for(int i=0;i<num;i++)
				code<<std::endl;
	}
	else
	{
		if(num > 0)
			code<<std::endl;
	}
	
}

//=================================================================================================
//Adds a line marker with filename and line number to sync C compiler output
//=================================================================================================
void CodeGenContext::place_line_marker( unsigned long line_num, const std::string& file )
{
	cur_filename = file;
	cur_line_num = line_num;

	if(gen_line_markers)
	{
		code<<std::endl<<"#line "<<line_num<<" \""<<file<<"\""<<std::endl;
	}
}

//=================================================================================================
//Adds a line marker with only the line number
//=================================================================================================
void CodeGenContext::place_line_marker( unsigned long line_num )
{
	cur_line_num = line_num;
	if(gen_line_markers)
	{
		code<<std::endl<<"#line "<<line_num<<std::endl;
	}
}

//=================================================================================================
//Adds line/filename markers or shifts the code with newlines to sync C compiler output
//=================================================================================================
void CodeGenContext::set_location( unsigned long line_num, const std::string& file )
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

//=================================================================================================
//Prints a C format string to the code
//Updates current line number
//=================================================================================================
void CodeGenContext::to_code_fmt( const char* format, ... )
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

//=================================================================================================
//Prints a C++ string to the code
//Does not keep track of newlines in the output - str must not contain any newlines
//=================================================================================================
void CodeGenContext::to_code( const std::string& str )
{
	ICASSERT(0 == std::count(str.begin(), str.end(), '\n'));
	code<<str;
}

//=================================================================================================
//Prints a C++ string to the code
//Updates current line number
//=================================================================================================
void CodeGenContext::to_code_string( const std::string& str )
{
	//keep track of newlines
	cur_line_num += std::count(str.begin(), str.end(), '\n');
	code<<str;
}

//=================================================================================================
//Prints atomic block header to the code
//"ATOMIC_BLOCK(ATOMIC_FORCEON){"
//=================================================================================================
void CodeGenContext::atomic_header()
{
	to_code_fmt("\n");
	indent();
	to_code_fmt("%s\n", C_ATOMIC_BLOCK_START);
	indent_depth++;
	//indent();
}

//=================================================================================================
//Prints atomic block footer in the code
//}
//=================================================================================================
void CodeGenContext::atomic_footer()
{
	to_code_fmt("\n");
	indent_depth--;
	indent();
	to_code_fmt("%s\n", C_ATOMIC_BLOCK_END);
}