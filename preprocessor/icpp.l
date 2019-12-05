
%x incl_path
%x incl_newline

%x def_tok
%x def_params
%x def_val
%x def_comment
%x def_comment_tail
%x def_args_lp
%x def_args

%x comment
%x inl_comment

%x ifdef_tok
%x ifdef_skip
%s ifdef_body
%x ifndef_tok

%x undef

%option noyywrap

%{
#include <iostream>
#include <stack>
#include <deque>
#include <map>
#include <algorithm>
#include <set>
#include <list>
#include <vector>
#include <sstream>
#include <string>
#define MAX_INCLUDE_DEPTH 100

bool pp_gen_line_markers = true;
bool pp_retain_comments = false;
extern FILE *output_file;

std::string comment_acc;//test

//These store current context
std::stack<YY_BUFFER_STATE> includes;
std::stack<unsigned int> line_nums;
std::deque<std::string> filenames;
unsigned long cur_line_num = 1;
std::string cur_filename;
std::set<std::string> once_headers; //list of headers with #pragma once

class ParamInsertion
{
public:
	ParamInsertion(unsigned int position, unsigned int param_id) : pos(position), pid(param_id) {}
	unsigned int pos;//occurrence position in define value string
	unsigned int pid;//param id
};

class Macro
{
public:
	Macro() : num_params(0) {}
	Macro(const std::string& name, const std::string& value, unsigned int num_params) : name(name), value(value), num_params(num_params) {}
	std::string name;
	std::string value;//for macros with params, no param names are present in this string
	std::list<ParamInsertion> param_inserts;
	unsigned int num_params;
	
	//Inserts arg values where respective parameters occur in the macro
	std::string substitute(const std::vector<std::string>& args) const
	{
		std::ostringstream os;
		os << value.substr(0, param_inserts.front().pos); //the part before any param entries
		for(std::list<ParamInsertion>::const_iterator it = param_inserts.begin(); it != param_inserts.end(); it++)
		{
			std::list<ParamInsertion>::const_iterator next = it; next++;
			if(param_inserts.end() == next) //last param entry and the rest of the macro value
				os << args[it->pid] << value.substr(it->pos, value.size() - it->pos);
			else //current param entry and substring till next param entry
				os << args[it->pid] << value.substr(it->pos, next->pos - it->pos);
		}
		return os.str();
	}
};

//Map of all defined macros - with or w/o parameters
typedef std::map<std::string, Macro> DefMap;
DefMap defines;

//Utility lists and objects - used to pass data between states
std::string cur_def_token;
std::string cur_def_value;//accumulator - make sure it's cleared each time
std::vector<std::string> cur_def_params; //current params in define, accumulator - make sure it's cleared each time
unsigned int cur_def_val_pos = 0;//Used in macro construction to track positions for param inserts, accumulator - make sure it's cleared each time
std::list<ParamInsertion> cur_param_inserts;//Used in macro construction, accumulator - make sure it's cleared each time
std::vector<std::string> cur_macro_args;//accumulator - make sure it's cleared each time
std::string cur_incl_path;
Macro cur_macro;//Used in macro instantiation

//Trimmer functions
const std::string WHITESPACE = " \t";
std::string ltrim(const std::string& s) {
	size_t start = s.find_first_not_of (WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}
std::string rtrim(const std::string& s) {
	size_t end = s.find_last_not_of (WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
std::string trim(const std::string& s){
	return rtrim(ltrim(s));
}

%}

id 		[a-zA-Z_][a-zA-Z0-9_]*
dconst 	[0-9]+\.[0-9]*          
iconst 	[0-9]+          		
hconst 	"0x"[a-fA-F0-9]+		
bconst 	"0b"[0-1]+				
cha 	'(\\.|[^\\'])+'			
str 	\"(\\.|[^\\"])*\"	 
punct 	"("|")"|"["|"]"|"{"|"}"|":"|";"|"="|"<"|">"|"."|","|"+"|"-"|"*"|"/"|"%"|"~"|"!"|"|"|"&"|"^"	
not_id	{dconst}|{iconst}|{hconst}|{bconst}|{cha}|{str}|{punct}

%%

" "					{ fputs(yytext, output_file); }
"\t"				{ fputs(yytext, output_file); }
"\n"				{ cur_line_num++; fputs(yytext, output_file); }
{id}				{
						//Identifier found - check if a macro
						DefMap::iterator it = defines.find(yytext);
						if(defines.end() == it)//not a macro
						{
							fputs(yytext, output_file);
						}
						else // macro - check if it has params
						{
							Macro m = it->second;
							if(0 == m.num_params)
							{
								fputs(m.value.c_str(), output_file);
							}
							else
							{
								BEGIN(def_args_lp);
								cur_macro = m;
							}
						}
					}
<def_args_lp>"("	{ BEGIN(def_args); cur_macro_args.clear(); }
<def_args_lp>[^"("]	{ std::cout<<cur_filename<<":"<<cur_line_num<<": error: expected macro arguments "<<std::endl; }
<def_args>[ \t]*([^,)\n]+)[ \t]*(")"|",") {//macro arguments, comma-separated
						std::string arg = ltrim(yytext);
						bool is_last_arg = (')' == *arg.rbegin());//check for closing parenthesis before trimming it off
						arg.erase(arg.end()-1, arg.end());//trim the comma or parenthesis
						arg = rtrim(arg);
						cur_macro_args.push_back(arg);
						
						if(is_last_arg)
						{
							if(cur_macro_args.size() < cur_macro.num_params)
								std::cout<<cur_filename<<":"<<cur_line_num<<": error: not enough arguments in macro \""<<cur_macro.name<<"\""<<std::endl;
							else if(cur_macro_args.size() > cur_macro.num_params)
								std::cout<<cur_filename<<":"<<cur_line_num<<": error: too many arguments in macro \""<<cur_macro.name<<"\""<<std::endl;
							else
								fputs(cur_macro.substitute(cur_macro_args).c_str(), output_file);
							BEGIN(INITIAL);
						}
					}
<def_args>\n		{ 
						std::cout<<cur_filename<<":"<<cur_line_num<<": error: syntax error in macro parameters. Forgot the \")\"?"<<std::endl;
						cur_line_num++;
						BEGIN(INITIAL);
					}
					
[^ \t\n]	       	{ fputs(yytext, output_file); }

 /****************************************************************************/
 /*          P R A G M A  O N C E                                            */
 /****************************************************************************/
^"#pragma once"[ \t]*"\n"	{
								std::set<std::string>::iterator it = once_headers.find(cur_filename);
								if(once_headers.end() == it)
								{
									cur_line_num++; 
									once_headers.insert(cur_filename);
								}
								else
								{
									//TODO: this code is same as in <<EOF>>
									if ( 0 == includes.size() )
									{
										yyterminate();
									}
									else
									{
										yy_delete_buffer( YY_CURRENT_BUFFER );
										
										//restore context from stacks
										yy_switch_to_buffer( includes.top() ); includes.pop();
										cur_line_num = line_nums.top(); line_nums.pop();
										cur_filename = filenames.back(); filenames.pop_back();
										
										//line marker
										if(pp_gen_line_markers)
											fprintf(output_file, "\n# %d \"%s\"\n", cur_line_num, cur_filename.c_str());
									}
								}
							}
 
 /****************************************************************************/
 /*                    C O M M E N T S                                       */
 /****************************************************************************/
 /*TODO: these comments can be stored for use in further translation*/
"//".*						{ 
								//comment_acc.append(yytext);//test
								if(pp_retain_comments)
									fputs(yytext, output_file);
								BEGIN(inl_comment);
							}
<inl_comment>\n				{  
								//comment_acc.append(yytext);//test
								cur_line_num++; 
								
								if(pp_retain_comments)
									fputs(yytext, output_file);
								else
									fputs("\n", output_file);
								BEGIN(INITIAL);
								
								//test
								//std::cout<<"merged inline comments:"<<std::endl<<comment_acc<<std::endl<<"end of merged inline comment"<<std::endl;//test
								//comment_acc.clear();
							}
 /*merge consequent inline comments*/
<inl_comment>\n[ \t]*"//".* { 
								//comment_acc.append(yytext);//test
								if(pp_retain_comments)
									fputs(yytext, output_file);
								else
									fputs("\n", output_file); 
								cur_line_num++; 
							} 

"/*"				{ 
						BEGIN(comment); 
						
						if(pp_retain_comments)
							fputs(yytext, output_file);
					}
<comment>\n			{ 
						cur_line_num++; 
						if(pp_retain_comments)
							fputs(yytext, output_file);
						else
							fputs("\n", output_file); 
					}
<comment>.			{
						if(pp_retain_comments)
							fputs(yytext, output_file);
					}
<comment>"*/"		{ 
						BEGIN(INITIAL); 
						if(pp_retain_comments)
							fputs(yytext, output_file);
					}
 /*merge consequent multiline comments*/
<comment>"*/"[ \t]*\n?[ \t]*"/*" 	{
										cur_line_num++; 
										if(pp_retain_comments)
											fputs(yytext, output_file);
										else
											fputs("\n", output_file); 
									} 

 /****************************************************************************/
 /*                    I F D E F / I F N D E F / E N D I F                   */
 /****************************************************************************/
^"#ifdef"[ \t]+			{ BEGIN(ifdef_tok); }
<ifdef_tok>{id}			{
							DefMap::iterator it = defines.find(yytext);
							if(defines.end() == it)
								BEGIN(ifdef_skip);
							else 
								BEGIN(ifdef_body);
						}
<ifdef_skip>^"#endif"	{ 
							if(pp_gen_line_markers)
								fprintf(output_file, "# %d \"%s\"\n", cur_line_num, cur_filename.c_str());							
							BEGIN(INITIAL);
						}
<ifdef_skip>.			;
<ifdef_skip>\n 			{ cur_line_num++; }
<ifdef_body>^"#endif"	{ BEGIN(INITIAL); } /*inclusive state - global rules work here*/
 
^"#ifndef"[ \t]+		{ BEGIN(ifndef_tok); }
<ifndef_tok>{id}		{
							DefMap::iterator it = defines.find(yytext);
							if(defines.end() != it){
								BEGIN(ifdef_skip);} /*shared with ifdef*/
							else 
								BEGIN(ifdef_body); /*shared with ifdef*/
						}

^"#endif"				{ std::cout<<cur_filename<<":"<<cur_line_num<<": error: stray #endif in program"<<std::endl; }

^"#undef"[ \t]+			{ BEGIN(undef); }
<undef>{id}				{
							DefMap::iterator it = defines.find(yytext);
							if(defines.end() != it)//defined
							{
								defines.erase(it);
							}
							BEGIN(INITIAL);
						}

 /****************************************************************************/
 /*                    D E F I N E                                           */
 /****************************************************************************/
^"#define"[ \t]+	{
						BEGIN(def_tok);
						cur_def_value.clear();
						cur_def_val_pos = 0; 
						cur_def_params.clear();
						cur_param_inserts.clear();
					}
<def_tok>{id}(("(")|([ \t]+))	{	
						std::string tok(yytext);
						tok = trim(tok);
						
						bool has_params = ('(' == *tok.rbegin());
						if(has_params)
							tok.erase(tok.end()-1, tok.end());//trim parenthesis
						
						if(defines.end() != defines.find(tok))
							std::cout << cur_filename << ":" << cur_line_num << ": error: " << "macro redefinition" << std::endl;
						
						cur_def_token = tok;
						BEGIN(has_params ? def_params : def_val);
					}		
<def_params>[ \t]*{id}[ \t]*((")"[ \t]+)|(",")) { 
						std::string param = trim(yytext);
						bool last_param = (')' == *param.rbegin());//need to check parenthesis before trimming it off
						param.erase(param.end()-1, param.end());//trim parenthesis
						param = rtrim(param);
						
						if(cur_def_params.end() == find(cur_def_params.begin(), cur_def_params.end(), param))//no param redefinition
							cur_def_params.push_back(param);
						else //param redefinition
							std::cout<<cur_filename<<":"<<cur_line_num<<": "<<"error: macro param \""<<param<<"\" already defined"<<std::endl;
						if(last_param)
							BEGIN(def_val);
					}
<def_val>{id}		{
						//Check if param
						bool is_param = false;
						if(0 != cur_def_params.size())
						{
							//Param ids are removed from the macro value string
							//We build a list of param insertions(occurrences), storing the position in resulting macro value
							//and id of the param which was there.
							//Param ids are their numbers(indices) in the params list in macro definition
							std::vector<std::string>::iterator it = find(cur_def_params.begin(), cur_def_params.end(), yytext);
							if(cur_def_params.end() != it) //found
							{
								is_param = true;
								ParamInsertion pins(cur_def_val_pos, it - cur_def_params.begin());
								cur_param_inserts.push_back(pins);
							}
						}
						
						if(!is_param)
						{
							//Check if it's a macro
							DefMap::iterator it = defines.find(yytext);
							std::string to_text = defines.end() != it ? it->second.value : yytext;
							cur_def_value.append(to_text);
							cur_def_val_pos += to_text.size();
							//TODO: handle (nested) parametric macros - same way as in the global {id} rule
						}
					}
<def_val>.			{
						std::string s = yytext;
						cur_def_value.append(s);
						cur_def_val_pos+=s.size(); 
					}	
<def_val>\n			{
						cur_line_num++;
						fputs("\n", output_file);
						
						Macro m(cur_def_token, cur_def_value, cur_def_params.size());
						if(0 != cur_def_params.size())
							m.param_inserts = cur_param_inserts;
						
						defines[cur_def_token] = m;
						
						BEGIN(INITIAL);
					}
					
 /*Comments in macro (these are useless for further translation)*/
<def_val>"/*"		{
						BEGIN(def_comment); 
					}	
<def_val>"//".*		{ 
						//getting an inline comment already means the define is done (multiline defines not supported)
						BEGIN(inl_comment);
						
						Macro m(cur_def_token, cur_def_value, cur_def_params.size());
						if(0 != cur_def_params.size())
							m.param_inserts = cur_param_inserts;
							
						defines[cur_def_token] = m;
						cur_def_value.clear();
					}					

 /*Multiline comments inside (and possibly over) the define value*/
<def_comment>.		{}
<def_comment>"*/"	{ BEGIN(def_val); }
<def_comment>\n		{
						cur_line_num++;
						fputs("\n", output_file);
						
						Macro m(cur_def_token, cur_def_value, cur_def_params.size());
						if(0 != cur_def_params.size())
							m.param_inserts = cur_param_inserts;
						
						defines[cur_def_token] = m;
						cur_def_value.clear();
						
						BEGIN(def_comment_tail);
					}
<def_comment_tail>.		{}
<def_comment_tail>"*/"	{ BEGIN(INITIAL); }
<def_comment_tail>\n	{
							cur_line_num++;
							fputs("\n", output_file);
						}
 
 /****************************************************************************/
 /*                    I N C L U D E                                         */
 /****************************************************************************/
^"#include"[ \t]+\" 	{ BEGIN(incl_path); }
<incl_path>[^\n\"]+\"  	{ 
							if( MAX_INCLUDE_DEPTH <= includes.size() )
							{
								std::cout << cur_filename << ":" << cur_line_num << ": error: " << "Max include file nesting depth exceeded"<<std::endl;
								BEGIN(INITIAL);
							}
							else
							{
								//trim quotation
								cur_incl_path = yytext;
								cur_incl_path = cur_incl_path.substr(0, cur_incl_path.size()-1);
								
								//cyclic dependency check - this is why filenames is a deque
								if(filenames.end() != find(filenames.begin(), filenames.end(), cur_incl_path))
								{
									std::cout << cur_filename << ":" << cur_line_num << ": error: " << "Cyclic dependency on \""<<cur_incl_path<<"\""<<std::endl;
									BEGIN(INITIAL);
								}
								else
								{
									BEGIN(incl_newline);
								}
							}
						}
<incl_newline>.			;						
<incl_newline>\n		{	
							cur_line_num++;
							yyin = fopen( cur_incl_path.c_str(), "r" );
							if ( ! yyin )
								std::cout << cur_filename << ":" << cur_line_num << ": error: " << "Can't find include file \""<<cur_incl_path<<"\""<<std::endl;
							
							//store context on stacks
							includes.push(YY_CURRENT_BUFFER);
							line_nums.push(cur_line_num);
							filenames.push_back(cur_filename);
							
							//create new context
							yy_switch_to_buffer( yy_create_buffer( yyin, YY_BUF_SIZE ) );
							cur_line_num = 1;
							cur_filename = cur_incl_path;
							
							BEGIN(INITIAL);
							
							//line marker
							if(pp_gen_line_markers)
								fprintf(output_file, "# %d \"%s\"\n", cur_line_num, cur_filename.c_str());
						}						
<<EOF>> 				{
							if ( 0 == includes.size() )
							{
								yyterminate();
							}
							else
							{
								yy_delete_buffer( YY_CURRENT_BUFFER );
								
								//restore context from stacks
								yy_switch_to_buffer( includes.top() ); includes.pop();
								cur_line_num = line_nums.top(); line_nums.pop();
								cur_filename = filenames.back(); filenames.pop_back();
								
								//line marker
								if(pp_gen_line_markers)
									fprintf(output_file, "\n# %d \"%s\"\n", cur_line_num, cur_filename.c_str());
							}
						}
		