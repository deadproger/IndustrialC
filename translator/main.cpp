#include <iostream>
#include <sstream>
#include <stack>
#include "iCProgram.h"
#include "iCScope.h"
#include "ParserContext.h"
#include "CodeGenContext.h"
#include <fstream>
#include <typeinfo>

///////////////////////////////////////////////////////////////////////////////
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/graphml.hpp>
typedef struct Vertex
{ 
	std::string name; 
	std::string cmdb_id;

	Vertex& operator= (const Vertex& rhs)
	{
		if (this == &rhs)
			return *this;

		name = rhs.name;
		cmdb_id = rhs.cmdb_id;
	}

	bool operator< (const Vertex& rhs) const
	{
		return cmdb_id < rhs.cmdb_id;
	};

	bool operator== (const Vertex& rhs) const
	{
		return ((cmdb_id == rhs.cmdb_id) && (name == rhs.name));
	};

}vertex_container;

typedef struct Edge {std::string name;} edge_container;

typedef boost::directed_graph<vertex_container, edge_container> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
///////////////////////////////////////////////////////////////////////////////

extern iCProgram* ic_program ;
extern int ic_parse();
extern FILE *ic_in;

std::string input_filename = "input.iprepr";
std::string output_filename = "output.cpp";
extern ParserContext* parser_context;
extern bool had_errors;
extern bool gen_line_markers;

/*struct ProcGraphNode
{
	ProcGraphNode(const std::string& first, const std::string& second) : first(first), second(second){}
	std::string first;
	std::string second;
};*/


std::set<ProcGraphNode> proc_graph;

int main(int argc, char **argv)
{
	///////////////////////////////////////////////////////////////////////////////
	Graph g;
	vertex_container A, B;
	edge_container AB;

	A.name="A";
	A.cmdb_id="1";
	B.name="B";
	B.cmdb_id="2";
	AB.name="A-B";

	vertex_descriptor v0 = g.add_vertex(A);
	vertex_descriptor v1 = g.add_vertex(B);
	g.add_edge(v0,v1,AB);

	boost::dynamic_properties dp;
	dp.property("vertex_name",get(&vertex_container::name,g));
	dp.property("vertex_cmdb_id",get(&vertex_container::cmdb_id,g));
	dp.property("edge_name",get(&edge_container::name,g));

	write_graphml(std::cout, g, dp);
	///////////////////////////////////////////////////////////////////////////////


	//1: Parse command line arguments
	if(argc <= 1)
	{
		std::cout<<"Error: no input file specified"<<std::endl;
		return -1;
	}
	input_filename = argv[argc-1];
	for(int i=1;i<argc-1;i++)
		if("-no-line-markers" == std::string(argv[i]))
			gen_line_markers = false;
		else if("-o" == std::string(argv[i]) && i+1 < argc)
			output_filename = argv[++i];

	//2: open a file handle to a particular file:
	FILE *myfile = fopen(input_filename.c_str(), "r");
	if (!myfile) // make sure it's valid:
	{
		std::cout << "Can't find " << input_filename << std::endl;
		return -1;
	}
	ic_in = myfile;// set flex to read from it instead of defaulting to STDIN:
	
	//3: parse the input source code
	parser_context = new ParserContext;//create parser context; 
	std::cout<<"parsing..."<<std::endl;
    int parse_result = ic_parse();//parse
	fclose(myfile);

	if((IC_RETURN_ERROR != parse_result) && (!had_errors))
	{
		//4: secondary (post-parsing) analysis
		//Nodes that need second-pass analysis have by now been to a second_pass_nodes list in parser context
		std::cout<<"post-parse analysis..."<<std::endl;
		for(std::set<iCNode*>::iterator i=parser_context->second_pass_nodes.begin();i!=parser_context->second_pass_nodes.end();i++)
		{
			(*i)->second_pass();
		}

		//5: code generation
		//Every node has a gen_code function, calling gen_code of its subnodes
		//All code generation is done via CodeGenContext methods
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

	//DOT process graph test
	/*
	std::cout<<proc_graph.size()<<std::endl;
	for(std::set<ProcGraphNode>::const_iterator i = proc_graph.begin();i != proc_graph.end(); i++)
	{
		std::cout<< i->first << "->" << i->second << ";" << std::endl;
	}
	*/
	proc_graph.clear();

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
