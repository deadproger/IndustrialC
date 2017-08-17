#include <iostream>
#include <sstream>
#include <stack>
#include "iCProgram.h"
#include "iCScope.h"
#include "ParserContext.h"
#include "CodeGenContext.h"
#include <fstream>
#include <typeinfo>
extern iCProgram* ic_program ;
extern int ic_parse();
extern FILE *ic_in;

std::string input_filename = "input.iprepr";
std::string output_filename = "output.cpp";
extern ParserContext* parser_context;
extern bool had_errors;

int main(int argc, char **argv)
{
	//Parse command line arguments
	if(argc <= 1)
	{
		std::cout<<"Error: no input file specified"<<std::endl;
		return -1;
	}
	input_filename = argv[argc-1];
	for(int i=1;i<argc-1;i++)
	{
		if(std::string(argv[i]) == "-o" && i+1 < argc)
		{
			output_filename = argv[++i];
		}
	}

	// open a file handle to a particular file:
	FILE *myfile = fopen(input_filename.c_str(), "r");
	
	// make sure it's valid:
	if (!myfile) 
	{
		std::cout << "Can't find " << input_filename << std::endl;
		return -1;
	}

	// set flex to read from it instead of defaulting to STDIN:
	ic_in = myfile;

	std::cout<<"parsing..."<<std::endl;

	//create parser context; 
	parser_context = new ParserContext;

	//parse
    int parse_result = ic_parse();

	fclose(myfile);

	if((IC_RETURN_ERROR != parse_result) && (!had_errors))
	{
		//secondary (post-parsing) analysis
		std::cout<<"post-parse analysis..."<<std::endl;
		for(std::set<iCNode*>::iterator i=parser_context->second_pass_nodes.begin();i!=parser_context->second_pass_nodes.end();i++)
		{
			(*i)->second_pass();
		}

		//code generation
		std::ofstream output_file;
		output_file.open(output_filename.c_str());

		CodeGenContext context(output_file, ic_program->get_hps());
		std::cout<<"generating code..."<<std::endl;
		ic_program->gen_code(context);

		output_file.close();
	}
	delete ic_program;
	delete parser_context;

	std::cout<<"translator finished"<<std::endl;

#ifdef ICDEBUG
	std::cout<<"pointers still allocated: "<<get_num_allocs()<<std::endl;
#endif

	//return 1 if parsing went with errors
	//this is needed for external application (IDE)
	//to know to terminate the build process
	if(had_errors)
		return 1;

    return 0;
}
