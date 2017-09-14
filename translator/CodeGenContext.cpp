#include "CodeGenContext.h"

bool gen_line_markers = true;

void CodeGenContext::add_new_lines( int num /*= 1*/ )
{
	cur_line_num += num;

	if(num > NEW_LINES_THRESHOLD)
		place_line_marker(cur_line_num);
	else
		for(int i=0;i<num;i++)
			code<<std::endl;
}

void CodeGenContext::place_line_marker( unsigned long line_num, const std::string& file )
{
	if(!gen_line_markers)
		return;
	cur_filename = file;
	cur_line_num = line_num;
	code<<std::endl<<"#line "<<line_num<<" \""<<file<<"\""<<std::endl;
}

void CodeGenContext::place_line_marker( unsigned long line_num )
{
	if(!gen_line_markers)
		return;
	cur_line_num = line_num;
	code<<std::endl<<"#line "<<line_num<<std::endl;
}

void CodeGenContext::set_location( unsigned long line_num, const std::string& file )
{
	if(!gen_line_markers)
	{
		//to_code_fmt("\n");
		return;
	}
	if(file != cur_filename)
	{
		place_line_marker(line_num, file);
	}
	else
	{
		if(line_num < cur_line_num)
		{
			place_line_marker(line_num);
		}
		else
		{
			add_new_lines(line_num - cur_line_num);
		}
	}
}

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

void CodeGenContext::to_code( const std::string& str )
{
	code<<str;
}

void CodeGenContext::to_code_string( const std::string& str )
{
	//keep track of newlines
	cur_line_num += std::count(str.begin(), str.end(), '\n');
	code<<str;
}