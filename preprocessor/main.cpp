#include <iostream>
#include <sstream>
#include <stack>
#include <fstream>

extern int yylex();
extern FILE *yyin;
extern std::string cur_filename;
extern bool pp_gen_line_markers;
extern bool pp_retain_comments;
//extern std::stack<unsigned int> line_nums;
//extern std::stack<std::string> filenames;
FILE *output_file;

int main(int argc, char **argv)
{
	std::string input_filename = "input.blah";
	std::string output_filename = "output.blah";

	//1: Parse command line arguments
	if(argc <= 1)
	{
		std::cout<<"Error: no input file specified"<<std::endl;
		return -1;
	}
	else
	{
		input_filename = argv[argc-1];
		for(int i=1;i<argc-1;i++)
			if("-no-line-markers" == std::string(argv[i]))
				pp_gen_line_markers = false;
			else if("-retain-comments" == std::string(argv[i]))
				pp_retain_comments = true;
			else if("-o" == std::string(argv[i]) && i+1 < argc)
				output_filename = argv[++i];
	}

	//2: open input file and set flex to read from it instead of defaulting to STDIN
	FILE *myfile = fopen(input_filename.c_str(), "r");
	if (!myfile)
	{
		std::cout << "Can't find " << input_filename << std::endl;
		return -1;
	}
	yyin = myfile;
	
	//line_nums.push(0);
	//filenames.push(input_filename);
	cur_filename = input_filename;
	
	output_file = fopen(output_filename.c_str(), "w+");
	if (!myfile)
	{
		std::cout << "Can't find " << output_filename << std::endl;
		return -1;
	}

	//3: parser pass
	//std::cout<<"parsing..."<<std::endl;
    int parse_result = yylex();
	fclose(myfile);
	fclose(output_file);
	//std::cout<<"parser finished"<<std::endl;
	
    return 0;
}


