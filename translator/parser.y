
%{
	#include "iCProgram.h"
	#include "iCScope.h"
	#include "ParserContext.h"
	#include "iCIdentifier.h"
	#include "iCMCUIdentifier.h"
	#include "iCIdentifierInProcType.h"
	#include "iCProcTypeParam.h"
	#include "iCProcTypeParamUsage.h"
	#include "iCProcType.h"
	#include "iCProcTypeInstantiation.h"
	#include "iCProcess.h"
	#include "iCState.h"
	#include "CCode.h"
	#include "iCTimeout.h"
	#include "iCHyperprocess.h"

	#include "iCStateTransition.h"
	#include "iCStartHPStatement.h"
	#include "iCStopHPStatement.h"
	#include "iCStartProcStatement.h"
	#include "iCStopProcStatement.h"
	#include "iCCompoundStatement.h"
	#include "iCSelectionStatement.h"
	#include "iCProcStatusCheck.h"

	#include "iCAssignmentExpression.h"
	#include "iCBinaryExpression.h"
	#include "iCCastExpression.h"
	#include "iCDouble.h"
	#include "iCExpressionStatement.h"
	#include "iCInteger.h"
	#include "iCLogicConst.h"
	#include "iCPostfixExpression.h"
	#include "iCPrimaryExpression.h"
	#include "iCUnaryExpression.h"
	#include "iCSubscriptExpression.h"
	#include "iCVariable.h"
	#include "iCVariableDeclaration.h"
	#include "iCFunction.h"
	#include "iCFunctionCall.h"
	#include "iCInitializer.h"
	#include "iCString.h"
	#include "iCIterationStatement.h"
	#include "iCAtomicBlock.h"
	#include "iCJumpStatement.h"
	#include "iCResetTimeoutStatement.h"
	#include "iCRestartStatement.h"

	#include <stdio.h>
	#include <stdarg.h> 
	#include <typeinfo>
	#include <set>
	
    iCProgram* ic_program = NULL; /* AST root */
	extern ParserContext* parser_context;

	//extern std::set<ProcGraphNode> proc_graph;//DOT graph test

    extern int ic_lex();
	int ic_error(const char *s);
%}

/***********************************************/
/*                DATA TYPES                   */
/***********************************************/
%union 
{
	iCIdentifier* ident;
	iCProgram* program;
	iCProcType* proctype;
	iCProcTypeInstantiation* proctype_instantiation;
	iCProcess* process;
	iCState* state;
	iCStatement* statement;
	CCode* ccode;
	NodesList* nodes_list;
	iCStateList* state_list;
	iCBlockItemsList* block_items_list;
	iCBlockItem* block_item;
	iCTimeout* timeout;
	iCExpression* expression;
	std::string *string;
	iCStringList *str_list;
	int token;
	iCVariablesList* var_list;
	iCHyperprocess* hyperprocess;
	iCDeclarationList* decl_list;
	iCProcessMap* proc_map;
	iCDeclaration* declaration;
	iCVariable* variable;
	iCIdentifierList* ident_list;
	iCProcTypeParamList* proctype_param_list;
	iCFunction* func;
	std::vector<iCExpression*>* expr_list;
	iCProgramItemsList* program_items_list;
	iCProgramItem* program_item;
	//iCProcBody* proc_body;
	iCInitializer* icinitializer;
	unsigned long line_num;
}

/***********************************************/
/*                   TOKENS                    */
/***********************************************/
%token <token> TPROCTYPE        "proctype"
%token <token> TPROC			"process"
%token <token> TSTATE			"state"
%token <token> TSTART			"start"
%token <token> TSTOP			"stop"
%token <token> TTIMEOUT			"timeout"
%token <token> TACTIVE			"active"
%token <token> TPASSIVE			"passive"
//%token <token> TSIZEOF			"sizeof"
%token <token> THYPERPROCESS	"hyperprocess"
%token <token> TVECTOR			"vector"	
%token <token> TREGISTER		"register"		
%token <token> TBIT				"bit"		
//%token <token> TENABLED			"enabled"	
%token <token> TSET				"set"		
//%token <token> TCLEAN			"clean"	
%token <token> TIF				"if"
%token <token> TELSE			"else"
%token <token> TSWITCH			"switch"
%token <token> TCASE			"case"
%token <token> TDEFAULT		"default"
%token <token> TFOR				"for"
%token <token> TATOMIC			"atomic"
%token <token> TRESET			"reset"

%token <token> TTRUE			"true"
%token <token> TFALSE			"false"
%token <string> TVOID			"void"		
%token <string> TCHAR			"char"		
%token <string> TINT			"int"			
%token <string> TSHORT			"short"		
%token <string> TLONG			"long"		
%token <string> TFLT			"float"			
%token <string> TDBL			"double"		
%token <string> TSGND			"signed"	
%token <string> TUNSGND			"unsigned"	
%token <string> TBOOL			"bool"	
%token <string> TCONST			"const"
%token <string> TVOLATILE		"volatile"
%token <string> TINLINE			"inline"
%token <string> TRETURN			"return"
%token <string> TBREAK			"break"
%token <string> TCONTINUE		"continue"

%token <string> TIDENTIFIER		"identifier"
%token <string> TICONST			"integer constant"
%token <string> TDCONST			"double constant"
%token <string> THCONST			"hex constant"
%token <string> TBCONST			"binary constant"
%token <string> TCCODELINE		"c code line"
%token <string> TCOMMENTLINE	"comment line"
%token <string> TCCODEEXPR		"c code expression"
%token <string> TSTRING			"string literal"
%token <token> TLPAREN			"(" 
%token <token> TRPAREN			")" 
%token <token> TLBRACKET		"["	
%token <token> TRBRACKET		"]"	
%token <token> TLBRACE			"{" 
%token <token> TRBRACE			"}" 
%token <token> TSEMIC			";" 
%token <token> TCOLON			":"
%token <string> TASSGN			"=" 
%token <string> TEQ				"=="
%token <string> TNEQ			"!="
%token <string> TLT				"<" 
%token <string> TLE				"<="
%token <string> TGT				">" 
%token <string> TGE				">="
%token <string> TDOT			"." 
%token <string> TCOMMA			"," 
%token <string> TPLUS			"+" 
%token <string> TMINUS			"-" 
%token <string> TASTERISK			"*" 
%token <string> TDIV			"/" 
%token <string> TPERC			"%"	
%token <string> TINC			"++"
%token <string> TDEC			"--"
%token <string> TRSHIFT			">>"
%token <string> TLSHIFT			"<<"
%token <string> TTILDE			"~"	
%token <string> TEXCLAM			"!"	
%token <string> TOR				"|"	
%token <string> TAND			"&"	
%token <string> TXOR			"^"	
%token <string> TLOR			"||"
%token <string> TLAND			"&&"
%token <string> TR_ASSGN		">>="	
%token <string> TL_ASSGN		"<<="	
%token <string> TPLUS_ASSGN		"+="	
%token <string> TMINUS_ASSGN	"-="	
%token <string> TASTERISK_ASSGN		"*="		
%token <string> TDIV_ASSGN		"/="		
%token <string> TPERC_ASSGN		"%="	
%token <string> TAND_ASSGN		"&="		
%token <string> TXOR_ASSGN		"^="		
%token <string> TOR_ASSGN		"|="	
%token <token>	TRESTART		"restart"

/***********************************************/
/*                   NODES                     */
/***********************************************/
%type <hyperprocess>		hp_definition
%type <proctype>            proctype_def
%type <proctype_instantiation> proctype_instantiation
%type <process>				proc_def
%type <state_list>			proc_body
%type <ccode>				c_code
%type <state>				state
%type <block_items_list>	state_body
%type <block_items_list>	block_items_list
%type <block_items_list>	state_items_list
%type <block_item>			state_block_item
%type <block_item>			block_item
%type <timeout>				timeout
%type <statement>			statement
%type <statement>			compound_statement
%type <statement>			func_body
%type <statement>			prep_compound
%type <statement>			expression_statement
%type <expression>			expr
%type <expression> 			assignment_expr
%type <expression> 			binary_expr
%type <expression> 			cast_expr
%type <expression> 			unary_expr
%type <expression> 			postfix_expr
%type <expression> 			primary_expr
%type <string> 				assignement_op
%type <var_list> 			var_declaration
%type <string> 				mcu_declaration
%type <str_list>			decl_specs			
%type <var_list>			init_declarator_list		
//%type <string>			init_declarator		
%type <variable>			direct_declarator					
%type <icinitializer>		initializer 
%type <icinitializer>		initializer_list
%type <string>				type_spec		
%type <func>				func_definition
%type <expr_list>			arg_expr_list
%type <variable>			init_declarator
%type <func>				func_declarator
%type <var_list>			param_list
%type <variable>			param_declarator
%type <statement>			for_init_statement
//%type <token>				for_prep_scope
%type <ident_list>			ident_list
%type <proctype_param_list>	proctype_param_list

%type <string>				program_items_list
%type <string>				program_item

%type <string>				type_name
%type <string>				abstract_declarator
%type <string>				direct_abstract_declarator
%type <string>				pointer

/***********************************************/
/*                  DESTRUCTORS                */
/***********************************************/
//these are to tell Bison how to properly clean up any non-terminals that
//had been left astray after an error
%destructor {} <token>
%destructor 
{
	while(!$$->empty())
	{
		delete $$->back();
		$$->pop_back();
	}
	delete $$;
}<block_items_list> //  <str_list>
%destructor 
{
	//smart pointers are used, no need to manually delete states
	delete $$;
}<state_list> 
%destructor
{
	delete $$;
}<expression>
%destructor 
{
	delete $$;
} <*>

/***********************************************/
/*              MISC DEFINITIONS               */
/***********************************************/
%start program /*Start symbol*/
%error-verbose /*Extended error reporting*/
%define api.prefix {ic_}

%nonassoc XIF
%nonassoc TELSE

//operator precedences
%left TLOR
%left TLAND
%left TOR
%left TXOR
%left TAND
%left TEQ TNEQ
%left TGT TLT TGE TLE
%left TLSHIFT TRSHIFT
%left TPLUS TMINUS
%left TASTERISK TDIV TPERC
%left TINC TDEC UMINUS TTILDE TEXCLAM

/*************************************************************************************************/
/*                                        GRAMMAR                                                */
/*************************************************************************************************/
//Bison uses flex to get next token (see tokens.l)
//Below is language grammar expressed in BNF/WSN-like notation.
//The notation is organized as follows:
//<non-terminal> : <rule1> | <rule2> | ... | <ruleN> ;
//Each rule has associated code that is executed when the rule is reduced
//Reducing the rule means applying it. That is only possible after all its non-terminals have been reduced by some other rules. 
//Bison parsing is done depth-first, i.e. AST leaves are the first ones to be reduced.
/*
Example (minimal IndustrialC program):
process p1 : background
{
	state FS_START
	{
		;
	}
}

With this example, the parser will descend through the program, process and 
state definitions, down to the ";" statement. The rule for the ";" statement
will then be reduced ( corresponding code executed). The parser will then go
up, reducing the rules for each node, until it hits program - the AST root.
So the code here kind'a executed from the bottom of AST to its top - keep that in mind.
*/

%%

program		:	{
					ic_program = new iCProgram(*parser_context);
					ic_program->add_hyperprocess(new iCHyperprocess("background", *parser_context));
					parser_context->set_program(ic_program);
					parser_context->add_to_second_pass(ic_program);
				}
				program_items_list 
				{
					//program scope is opened in ParserContext constructor, but closed here
					parser_context->close_scope();//end of the program - close the scope
					$2;
				}
			;

//nothing is really done here, because items get added to the program directly when parsed
program_items_list	:	program_items_list program_item { $$ = $1; $2; }
					|	program_item { $$ = NULL; $1; }
					;

program_item	:	var_declaration	//global var declarations
					{
						//split the declaration into separate variables and feed them to the program
						//the variables are global, var_declaration is a simple list of iCVariable
						//no iCVaribleDeclaration objects are built or used here
						for(iCVariablesList::iterator i=$1->begin();i!=$1->end();i++)
							ic_program->add_variable(*i);
						delete $1;
						$$ = NULL;
					}
				|	mcu_declaration	// mcu declaraions are "vector", "register" and "bit" usually found in mcu-specific .ih
					{
						//check for redefinition: error message is generated inside check_identifier_defined (not a very explicit method...)
						parser_context->check_identifier_defined(*$1);
						parser_context->add_mcu_decl_to_scope(*$1);
						delete $1;	
						$$ = NULL;
					}
				|	proc_def
					{
						if(NULL!=$1)ic_program->add_process($1);
						$$ = NULL;
					}
				|	proctype_def
					{
						if(NULL!=$1)ic_program->add_proctype($1);
						$$ = NULL;
					}
				|	proctype_instantiation
					{
						//iCProcTypeInstantiation will be also added to ic_program as iCProcess 
						// in iCProcTypeInstantiation::second_pass()
						if(NULL!=$1)ic_program->add_proctype_instantiation($1);
						$$ = NULL;
					}
				|	hp_definition // hyperprocess definitions with hp name, vector, register & bit
					{
						if(NULL!=$1)ic_program->add_hyperprocess($1);
						$$ = NULL;
					}
				|	c_code
					{
						if(NULL!=$1)ic_program->add_mcu_declaration($1);
						$$ = NULL;
					}
				|	func_definition
					{
						ic_program->add_function($1);
						$$ = NULL;
					}
				;

//=================================================================================================
//Hyperprocess definition defines the name, interrupt vector, as well as register and bit that 
//are used to enable/disable the interrupt
// * don't see the need for that midrule action here - look into and fix later
//=================================================================================================
/*
hyperprocess <hp_name> 
{
	vector=<vec_name>;
	register=<reg_name>;
	bit=<bit_name>;
}
*/
hp_definition	:	THYPERPROCESS TIDENTIFIER // 1  2
					{
						//check hyperprocess redefinition - if redefined, gen error and move on anyway
						if(ic_program->hp_defined(*$2)) 
							parser_context->err_msg("hyperprocess redefinition: %s already defined", $2->c_str());

						//iCHyperprocess is created with name only, params are parsed and added later 
						$<hyperprocess>$ = new iCHyperprocess(*$2, *parser_context);
						delete $2;
					}
					TLBRACE // 4
					TVECTOR		TASSGN TIDENTIFIER TSEMIC // 5 6 7 8
					TREGISTER	TASSGN TIDENTIFIER TSEMIC // 9 10 11 12
					TBIT		TASSGN TIDENTIFIER TSEMIC // 13 14 15 16
					TRBRACE	// 17
					{
						//check the vector, register and bit have been declared
						if(NULL == parser_context->get_mcu_decl_scope(*$7))
							parser_context->err_msg("undefined interrupt vector: %s ", $7->c_str());
						if(NULL == parser_context->get_mcu_decl_scope(*$11))
							parser_context->err_msg("undefined mcu register: %s ", $11->c_str());
						if(NULL == parser_context->get_mcu_decl_scope(*$15))
							parser_context->err_msg("undefined register bit: %s ", $15->c_str());
						
						//finalize the iCHyperprocess object
						$$ = $<hyperprocess>3;
						$$->set_params(*$7, *$11, *$15);

						delete $7; delete $11; delete $15;//params
						delete $6; delete $10; delete $14;//TASSGNs

						//suppress unused value warning
						$1;$4;$5;$8;$9;$12;$13;$16;$17;
					}
				;

//process type definition
proctype_def : TPROCTYPE TIDENTIFIER 
				TLPAREN TRPAREN 
				{
					printf("parser: entered proctype_def without parameters rule\n");
					//check for proctype redefinition
					if (ic_program->proctype_defined(*$2)) //todo: replace with parser_context->get_proctype_scope ?
						parser_context->err_msg("process type redefinition: %s already defined", $2->c_str());

					iCProcTypeParamList* empty_param_list = new iCProcTypeParamList();
					$<proctype>$ = new iCProcType(*$2, *empty_param_list, *parser_context);
					parser_context->set_proctype($<proctype>$); //entering proctype definition
					parser_context->open_scope(*$2); //enter proctype scope
					delete $2; //proctype name
					delete empty_param_list;
				}
				TLBRACE proc_body TRBRACE //version without parameters
				{
					//proc body has already been parsed - closing process scope
					parser_context->close_scope();

					//finalize the iCProcType and add it to scope
					//if a redifinition took place the proctype will have multiple instance in the scope but it's ok
					$$ = $<proctype>5;
					$$->add_states(*$7);
					parser_context->add_proctype_to_scope($$->name);

					//restore context
					parser_context->set_proctype(NULL); //leaving proctype definition
					//parser_context->leave_isr(); //todo:uncomment when isr will work

					delete $7; //proc body (list of states) //todo: what's a reason to delete it?
					$1; $3; $4; $6; $8;
				}
				| TPROCTYPE TIDENTIFIER //version with parameters
				TLPAREN proctype_param_list TRPAREN
				{
					printf("parser: entered proctype_def with parameters rule\n");
					//check for proctype redefinition
					if (ic_program->proctype_defined(*$2)) //todo: replace with parser_context->get_proctype_scope ?
						parser_context->err_msg("process type redefinition: %s already defined", $2->c_str());

					$<proctype>$ = new iCProcType(*$2, *$4, *parser_context);
					parser_context->set_proctype($<proctype>$); //entering proctype definition
					parser_context->open_scope(*$2); //enter proctype scope
					delete $2; //proctype name
					delete $4; //proctype params
				}
				TLBRACE proc_body TRBRACE
				{
					//proc body has already been parsed - closing process scope
					parser_context->close_scope();

					//finalize the iCProcType and add it to scope
					//if a redifinition took place the proctype will have multiple instance in the scope but it's ok
					$$ = $<proctype>6;
					$$->add_states(*$8);
					parser_context->add_proctype_to_scope($$->name);

					//restore context
					parser_context->set_proctype(NULL); //leaving proctype definition
					//parser_context->leave_isr(); //todo:uncomment when isr will work

					delete $8; //proc body (list of states)
					$1;$3;$5;$7;$9;
				};

//iCProcTypeParamList*
proctype_param_list: proctype_param_list TCOMMA TIDENTIFIER
			{
				parser_context->check_identifier_defined(*$3);

				$$ = $1;
				//todo: check if var for such id is defined (in proctype?), if yes then take scope from there
				const iCScope* scope = parser_context->get_current_scope();
				iCProcTypeParam* id = new iCProcTypeParam(*$3, scope, *parser_context);
				$$->push_back(id);
				parser_context->add_proctype_param_to_scope(id);
				delete $2;
				delete $3;
			}
				| TIDENTIFIER
			{
				parser_context->check_identifier_defined(*$1);

				$$ = new iCProcTypeParamList;
				//todo: check if var for such id is defined, if yes then take scope from there
				const iCScope* scope = parser_context->get_current_scope();
				iCProcTypeParam* id = new iCProcTypeParam(*$1, scope, *parser_context);
				$$->push_back(id);
				parser_context->add_proctype_param_to_scope(id);
				delete $1;
			};

//process type instantiation
proctype_instantiation: TIDENTIFIER TIDENTIFIER TLPAREN TRPAREN TSEMIC
						{
							printf("parser: entered proctype_instantiation rule\n");
							//check for process redefinition
							const iCScope* scope = parser_context->get_proc_scope(*$2);
							if (NULL != scope)
							{
								//process already defined - gen error, but continue parsing anyway to check for more errors
								parser_context->err_msg("process redefinition: %s already defined in %s",
									$2->c_str(), scope->name.empty() ? "this scope" : scope->name.c_str());
							}

							iCIdentifierList* empty_arg_list = new iCIdentifierList();
							$$ = new iCProcTypeInstantiation(ic_program, *$1, *$2, *empty_arg_list, *parser_context);
							$$->set_hp("background"); //todo: other hyperprocesses
							parser_context->add_proc_to_scope(*$2);
							parser_context->add_to_second_pass($$);

							delete $1;
							delete $2;
							delete empty_arg_list;
							$3; $4; $5;
						}
						| TIDENTIFIER TIDENTIFIER TLPAREN ident_list TRPAREN TSEMIC
						{
							printf("parser: entered proctype_instantiation rule\n");
							//check for process redefinition
							const iCScope* scope = parser_context->get_proc_scope(*$2);
							if (NULL != scope)
							{
								//process already defined - gen error, but continue parsing anyway to check for more errors
								parser_context->err_msg("process redefinition: %s already defined in %s",
									$2->c_str(), scope->name.empty() ? "this scope" : scope->name.c_str());
							}

							$$ = new iCProcTypeInstantiation(ic_program, *$1, *$2, *$4, *parser_context);
							$$->set_hp("background"); //todo: other hyperprocesses
							parser_context->add_proc_to_scope(*$2);
							parser_context->add_to_second_pass($$);

							delete $1;
							delete $2;
							delete $4;
							$3; $5; $6;
						};

//iCIdentifierList*
ident_list: ident_list TCOMMA TIDENTIFIER
					{
						$$ = $1;
						const iCScope* scope = parser_context->get_current_scope();
						iCIdentifier* id = new iCIdentifier(*$3, scope, *parser_context);
						$$->push_back(id);
						delete $2;
						delete $3;
					}
						| TIDENTIFIER
					{
						$$ = new iCIdentifierList;
						const iCScope* scope = parser_context->get_current_scope();
						iCIdentifier* id = new iCIdentifier(*$1, scope, *parser_context);
						$$->push_back(id);
						delete $1;
					};

//=================================================================================================
//iCProcess object needs to be created before the states are parsed,
//so that we have an already opened scope and a process in ParserContext when parsing the states.
//proc_body rule builds a list of states.
//proc_def rule creates iCProcess w/o states, then copies the states into iCProcess later.
//=================================================================================================
//process <proc_name> : <hp_name> { <proc_body> }
proc_def	:	TPROC TIDENTIFIER // 1 2
				{
					//check for process redefinition
					const iCScope* scope = parser_context->get_proc_scope(*$2);
					if(NULL != scope)
					{
						//process already defined - gen error, but continue parsing anyway to check for more errors
						parser_context->err_msg("process redefinition: %s already defined in %s",
												$2->c_str(), scope->name.empty()?"this scope":scope->name.c_str());
					}
					
					//Create the iCProcess objects (w/o states or activator) and modify context
					$<process>$ = new iCProcess(*$2, *parser_context);
					parser_context->set_process($<process>$);//entering process definition
					parser_context->open_scope(*$2);// enter process scope
					delete $2;
				}
				TCOLON TIDENTIFIER // 4 5
				{
					//hyperprocess defined check - hp needs to be defined beforehand
					if(!ic_program->hp_defined(*$5))
						parser_context->err_msg("undefined hyperprocess: %s", $5->c_str());
					
					//set process hp and modify context
					$<process>3->set_hp(*$5);
					if($<process>3->is_isr_driven())
						parser_context->enter_isr();

					delete $5;//hp name
				}
				TLBRACE proc_body TRBRACE // 7 8 9 
				{
					//proc body has already been parsed - closing process scope
					parser_context->close_scope();

					//finalize the iCProcess and add it to scope
					//if a redifinition took place the proc will have multiple instance in the scope but it's ok
					$$ = $<process>3;
					$$->add_states(*$8);
					parser_context->add_proc_to_scope($$->name);

					//restore context
					parser_context->set_process(NULL);//leaving process definition
					parser_context->leave_isr();
					
					delete $8;//proc body (list of states)
					$1;$4;$7;$9;//suppress unused value warning
				}
			;	

//=================================================================================================
//process body can contain state definitions and variable declarations (interleaved)
//states are stored in a list that is passed further as proc_body node
//var declarations are disbanded with variables added directly to program (as globals with scope prefixes)
//variables already know their context (the process they belong to)
//var_declaration is a simple list of vars. No iCVariableDeclaration objects are built here
//=================================================================================================
proc_body	: proc_body state 
			{
				//states are stored in the list
				$1->push_back($2);
				$$ = $1;
			}
			| proc_body var_declaration 
			{
				//Split the declaration into variables and feed them to the program
				iCVariablesList* vars = $2;

				iCProcType* proctype = parser_context->modify_proctype();
				if (NULL == proctype) //proc_body belongs to non-parameterized process
				{
					std::cout << "parser.body+var_decl branch: proc_body belongs to non-parameterized process" << std::endl;
					for (iCVariablesList::iterator i = vars->begin(); i != vars->end(); i++)
						ic_program->add_variable(*i);
				}
				else //proc_body belongs to proctype
				{
					std::cout << "parser.body+var_decl branch: proc_body belongs to proctype, proctype_name=" << proctype->name << std::endl;
					for (iCVariablesList::iterator i = vars->begin(); i != vars->end(); i++)
						proctype->add_variable(*i);
				}

				delete vars;
				$$ = $1;
			}
			| state 
			{
				$$ = new iCStateList();
				$$->push_back($1);
			}
			| var_declaration 
			{
				//Split the declaration into variables and feed them to the program
				iCVariablesList* vars = $1;

				iCProcType* proctype = parser_context->modify_proctype();
				if (NULL == proctype) //proc_body belongs to non-parameterized process
				{
					std::cout << "parser.var_decl branch: proc_body belongs to non-parameterized process" << std::endl;
					for (iCVariablesList::iterator i = vars->begin(); i != vars->end(); i++)
						ic_program->add_variable(*i);
				}
				else //proc_body belongs to proctype
				{
					std::cout << "parser.var_decl branch: proc_body belongs to proctype, proctype_name=" << proctype->name << std::endl;
					for (iCVariablesList::iterator i = vars->begin(); i != vars->end(); i++)
						proctype->add_variable(*i);
				}
				
				delete vars;
				$$ = new iCStateList();
			}
			;

//=================================================================================================
//the iCState object is created before parsing the body, so that body items have proper context
//states are stored in a list that is passed further as proc_body node
//var declarations are disbanded with variables added directly to program (as globals with scope prefixes)
//variables already know their context (the process they belong to)
//var_declaration is a simple list of vars. No iCVariableDeclaration objects are built here
//=================================================================================================
state		:	TSTATE TIDENTIFIER
				{
					//check for state redefinition
					const iCScope* scope = parser_context->get_state_scope(*$2);
					if(NULL != scope)
					{
						//state already defined - gen error message and move on like it's ok
						parser_context->err_msg("state redefinition: %s already defined in %s",
												$2->c_str(), scope->name.empty()?"this scope":scope->name.c_str());
					}

					//Create the state object with name only - state body is parsed and added later
					$<state>$ = new iCState(*$2, *parser_context);

					//modify context - we are now inside a state and inside a new scope
					parser_context->add_state_to_scope(*$2);
					parser_context->open_scope(*$2); 
					parser_context->set_state($<state>$);
					delete $2;
				}
				TLBRACE state_body TRBRACE // 4 5 6
				{
					//finalize the iCState object - copy parsed body items
					$$ = $<state>3; 
					if(NULL != $5)
						$$->set_items(*$5); 

					//restore context
					parser_context->close_scope(); 
					parser_context->set_state(NULL);

					delete $5;// state body
					$1;$2;$4;$6;//suppress unused value warning
				}
			;
		
//=================================================================================================
//the state body is a list of block items and (at most) one timeout. or it can be nothing (empty)
//the state_body rule is needed so that the body could be empty.
//state_items_list and state_block_item are needed to give special treatment to timeout
//TODO: should be able to simply use block_items_list directly instead of state_body
//could get rid of all those rules and add explicit checks instead of embedding them into the grammar
//=================================================================================================
state_body	:	%empty {$$ = new iCBlockItemsList();} // state body can be empty
			|	state_items_list {$$ = $1;}
			;

state_items_list	:	state_items_list state_block_item //build a list of state_block_items
						{
							if(NULL != $2)
								$1->push_back($2);
							$$=$1;
						}
					|	state_block_item 
						{
							$$ = new iCBlockItemsList(); 
							if(NULL != $1)
								$$->push_back($1);
						}
					|	state_items_list error TSEMIC { yyerrok; $$=$1; $3; }
					|	error TSEMIC { $$ = new iCBlockItemsList(); yyerrok; $2; }
					;

state_block_item	:	block_item	{$$ = $1;}
					|	timeout //timeout is treated separately from all the other statements
						{
							$$ = NULL;
							parser_context->modify_state()->set_timeout($1);//a hack to access the state
						}
					;
//iCBlockItem*
block_item	:	var_declaration //iCVariablesList*
				{
					// these are local variables - make sure we are inside a state or a function
					ICASSERT(NULL != parser_context->get_func() || NULL != parser_context->get_state());

					//Build an iCVariableDeclaration object and pass it further
					iCVariablesList* vars = $1;
					iCVariableDeclaration* decl = new iCVariableDeclaration(*parser_context);
					decl->set_vars(*vars);
					$$ = decl;
					delete vars;
				}
			|	statement {$$ = $<block_item>1;}
			|	c_code {$$ = $<block_item>1;}	
			;

block_items_list	:	block_items_list block_item //build a list of block items
						{
							if(NULL != $2)
								$1->push_back($2); 
							$$=$1;
						}
					|	block_item 
						{
							$$ = new iCBlockItemsList; 
							if(NULL != $1)
								$$->push_back($1);
						}
					;

c_code		:	TCCODELINE 
				{
					$$ = new CCode(*parser_context); 
					$$->add_line(new CCodeLine(*$1, *parser_context)); 
					delete $1;
				}
			;
			
/*************************************************************************************************/     
/*                                 S T A T E M E N T S                                           */     
/*************************************************************************************************/
statement	:	TSET TSTATE TIDENTIFIER TSEMIC //set state <state_name>;
				{
					const iCProcess* proc = parser_context->get_process();
					const iCProcType* proctype = parser_context->get_proctype();
					if(NULL == proc && NULL == proctype)
					{
						parser_context->err_msg("\"set state\" statement can only be used inside states");
						$$ = NULL;
					}
					else
					{
						$$ = new iCStateTransition(*$3, *parser_context); 
						parser_context->add_to_second_pass($$); // to check if state was defined
					}
					delete $3;
					$1;$2;$4;//suppress unused value warning
				}			
			|	TSTART TPROC TIDENTIFIER TSEMIC //start process <proc_name>;
				{
					const iCProcess* proc = parser_context->get_process();
					const iCProcType* proctype = parser_context->get_proctype();
					if(NULL == proc && NULL == proctype)
					{
						parser_context->err_msg("\"start process\" statement can only be used inside states");
						$$ = NULL;
					}
					else
					{
						$$ = new iCStartProcStatement(*$3, *parser_context); 
						parser_context->add_to_second_pass($$); // to check if process was defined

						//add edge to the process graph (for DOT generation)
						//proc_graph.insert(ProcGraphNode(proc->name, *$3));//DOT graph test
					}
					delete $3;
					$1;$2;$4;//suppress unused value warning
				}	
			|	TSTOP TPROC TIDENTIFIER TSEMIC //stop process <proc_name>;
				{
					const iCProcess* proc = parser_context->get_process();
					const iCProcType* proctype = parser_context->get_proctype();
					if (NULL == proc && NULL == proctype)
					{
						parser_context->err_msg("\"stop process\" statement can only be used inside states");
						$$ = NULL;
					}
					else
					{
						$$ = new iCStopProcStatement(*$3, *parser_context);
						parser_context->add_to_second_pass($$); // to check if process was defined

						//add edge to the process graph (for DOT generation)
						//proc_graph.insert(ProcGraphNode(proc->name, *$3));//DOT graph test
					}
					delete $3;
					$1;$2;$4;//suppress unused value warning
				}	
			|	TSTOP TPROC TSEMIC //stop process;
				{
					const iCProcess* proc = parser_context->get_process();
					const iCProcType* proctype = parser_context->get_proctype();
					if (NULL == proc && NULL == proctype)
					{
						parser_context->err_msg("\"stop process\" statement can only be used inside states");
						$$ = NULL;
					}
					else
					{
						if (NULL != proc)
						{
							$$ = new iCStopProcStatement(proc->name, *parser_context);
						}
						else
						{
							$$ = new iCStopProcStatement(*parser_context);
						}
						parser_context->add_to_second_pass($$); // to check if process was defined
					}
					$1;$2;$3;//suppress unused value warning
				}
			|	expression_statement //e.g. "a = b + c - d;"
			|	TSTART THYPERPROCESS TIDENTIFIER TSEMIC //start hyperprocess <hp_name>;
				{
					$$ = new iCStartHPStatement(*$3, *parser_context); 
					parser_context->add_to_second_pass($$); // to check if HP was defined
					delete $3;
					$1;$2;$4;//suppress unused value warning
				}
			|	TSTOP THYPERPROCESS TIDENTIFIER TSEMIC //stop hyperprocess <hp_name>;
				{
					$$ = new iCStopHPStatement(*$3, *parser_context); 
					parser_context->add_to_second_pass($$); // to check if HP was defined
					delete $3;
					$1;$2;$4;//suppress unused value warning
				}
			|	TSTOP THYPERPROCESS TSEMIC // stop hyperprocess;
				{
					const iCProcess* proc = parser_context->get_process();
					if(NULL == proc)
					{
						parser_context->err_msg("\"stop hyperprocess\" statement can only be used inside states");
						$$ = NULL;
					}
					else
					{
						$$ = new iCStopHPStatement(proc->activator, *parser_context); 
						parser_context->add_to_second_pass($$); // required for it to work
					}
					$1;$2;$3;//suppress unused value warning
				}
			|	compound_statement // {...}
			|	TIF TLPAREN expr TRPAREN statement TELSE statement { $<statement>$ = new iCIfElseStatement(*parser_context, $5, $7, $3); $1;$2;$4;$6;	}
			|	TIF TLPAREN expr TRPAREN statement %prec XIF { $<statement>$ = new iCIfElseStatement(*parser_context, $5, NULL, $3); $1;$2;$4; }
			|	TSWITCH TLPAREN expr TRPAREN statement {$<statement>$ = new iCSwitchStatement(*parser_context, $5, $3); $1;$2;$4; }
			|	TCASE expr TCOLON statement {$$ = new iCCaseStatement(*parser_context, $4, $2); $1; $3; }
			|	TDEFAULT TCOLON statement {$$ = new iCDefaultStatement(*parser_context, $3); $1; $2; }
				
				//for loop needs a preopened scope for variables declared in its init statement
				//for_prep_scope opens a separate scope that wraps the whole for statement
			|	TFOR
				for_prep_scope TLPAREN for_init_statement expression_statement expr TRPAREN statement
				{
					$$ = new iCIterationStatement($4, $5, $6, $8, *parser_context);
					parser_context->close_scope();
					$1;$3;$7;
				}
			|	TFOR for_prep_scope TLPAREN for_init_statement expression_statement TRPAREN statement
				{
					$$ = new iCIterationStatement($4, $5, NULL, $7, *parser_context);
					parser_context->close_scope();
					$1;$3;$6;
				}
			|	TATOMIC	statement // explicit atomic block
				{
					//Atomic block with FORCEON executed in ISR would cause nested interrupts
					if(parser_context->get_process()->is_isr_driven())
					{
						if(NULL != $2)
							delete $2;
						$$ = NULL;
						parser_context->err_msg("atomic block in interrupt-activated process");
					}
					else
					{
						iCAtomicBlock* atomic_block = new iCAtomicBlock(*parser_context);
						atomic_block->set_body($2);
						$$ = atomic_block;
					}
					$1;//suppress unused value warning
				}
			|	TRESET TTIMEOUT TSEMIC { $$ = new iCResetTimeoutStatement(*parser_context); $1;$2;$3;}
			|	TRETURN expr TSEMIC	{ $$ = new iCJumpStatement("return",	$2,		*parser_context);		delete $1; $3; }
			|	TRETURN TSEMIC		{ $$ = new iCJumpStatement("return",	NULL,	*parser_context);		delete $1; $2; }
			|	TBREAK TSEMIC		{ $$ = new iCJumpStatement("break",		NULL,	*parser_context); 		delete $1; $2; }
			|	TCONTINUE TSEMIC	{ $$ = new iCJumpStatement("continue",	NULL,	*parser_context);		delete $1; $2; }
			|	TRESTART TSEMIC { $$ = new iCRestartStatement(*parser_context); $1;$2; }
			;

//subroutine to open a "for" loop scope before parsing the init statement
for_prep_scope		:	%empty { parser_context->open_scope("for"); } 
					;

for_init_statement	:	expression_statement 
					|	var_declaration 
						{
							iCVariableDeclaration* decl = new iCVariableDeclaration(*parser_context);
							decl->set_vars(*$1);
							$$ = decl;
							delete $1;
						}
					;

expression_statement: expr TSEMIC { $$ = new iCExpressionStatement($1, *parser_context); $2; }
					| TSEMIC { $$ = new iCExpressionStatement(NULL, *parser_context); $1; }

//compound statement is pair of braces with block items in between. e.g. for or if body can be a compound statement
//prep_compound rule is used to open a separate scope for the items inside
//TODO: use midrule action instead of prep_compound, remove prep_compound in the empty case - don't need the scope there.
compound_statement:		TLBRACE prep_compound block_items_list TRBRACE 
						{
							ICASSERT(NULL != $3)
							$$ = $2;
							static_cast<iCCompoundStatement*>($<statement>$)->set_items(*$3);
							parser_context->close_scope();
							delete $3;
							$1;$4;//suppress unused value warning
						}
					|	TLBRACE prep_compound TRBRACE // empty compound statement {}
						{
							$$ = $2;
							parser_context->close_scope();
							$1;$3;
						}
					;
prep_compound:	%empty { $$ = new iCCompoundStatement(*parser_context); parser_context->open_scope("comp"); }; // dummy rule to prepare scope for compound statement

//iCTimeout is created without body to prep the scope for the body items
//enter_timeout and leave_timeout are used in critical section (atomic block) placement analysis
timeout	:	TTIMEOUT TLPAREN expr TRPAREN // 1 2 3 4
			{
			  //created only with name, items are parsed and added later
			  $<timeout>$ = new iCTimeout($3, *parser_context);
			  
			  //modify context
			  parser_context->open_scope("timeout");
			  parser_context->enter_timeout();
			}
			TLBRACE block_items_list TRBRACE // 6 7 8
			{
			  //finalize the iCTimeout object
			  $$ = $<timeout>5;
			  $$->set_items(*$7);
			  
			  //restore the scope
			  parser_context->close_scope();
			  parser_context->leave_timeout();
			  delete $7; // block_items_list
			  $1;$2;$4;$6;$8;//suppress unused value warning
			}
		;
			
/*************************************************************************************************/     
/*                                 E X P R E S S I O N S                                         */     
/*************************************************************************************************/
//iCExpression*
expr 		: assignment_expr 
	 		;
//iCExpression*
assignment_expr : binary_expr 
				| unary_expr assignement_op assignment_expr {$$ = new iCAssignmentExpression($1, *$2, $3, *parser_context); delete $2;}
				;
//iCExpression*
binary_expr : cast_expr 
			| binary_expr	 TLOR		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TLAND		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TOR		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TXOR		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TAND		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TEQ		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TNEQ		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TLT		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TGT		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TLE		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TGE		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TLSHIFT	binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TRSHIFT	binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TPLUS		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TMINUS		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TASTERISK		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TDIV		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TPERC		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			;
//iCExpression*
unary_expr 	: postfix_expr 
		   	| TINC unary_expr {$$ = new iCUnaryExpression(*$1, $2, *parser_context); delete $1;}
		   	| TDEC unary_expr {$$ = new iCUnaryExpression(*$1, $2, *parser_context); delete $1;}
		   	| TMINUS cast_expr %prec UMINUS {$$ = new iCUnaryExpression(*$1, $2, *parser_context); delete $1;}
			| TTILDE cast_expr {$$ = new iCUnaryExpression(*$1, $2, *parser_context); delete $1;}
			| TEXCLAM cast_expr {$$ = new iCUnaryExpression(*$1, $2, *parser_context); delete $1;}
		   	/*| TSIZEOF unary_expr {$$ = new iCUnaryExpression($1, $2);}*/
		   	/*| TSIZEOF type_name {$$ = new iCUnaryExpression($1, $2);}*/
		   	;
//iCExpression*
postfix_expr : primary_expr 
			 | postfix_expr TLBRACKET expr TRBRACKET // array indexing
			   {
				   $$ = new iCSubscriptExpression($1, $3, *parser_context);
				   $2;$4;
			   }
			 |	TIDENTIFIER TLPAREN TRPAREN // function call w/o arguments
			   {
				   $$ = new iCFunctionCall(*$1, *parser_context);
				   delete $1;
				   $2;$3;
			   }
			 |	 TIDENTIFIER TLPAREN arg_expr_list TRPAREN // function call w/ arguments
				 {
					 iCFunctionCall* func_call = new iCFunctionCall(*$1, *parser_context);
					 func_call->set_args(*$3);
					 $$ = func_call;
					 
					 delete $1;
					 delete $3;
					 $2;$4;
				 }
			 | postfix_expr TINC {$$ = new iCPostfixExpression($1, *$2, *parser_context); delete $2;} // --
			 | postfix_expr TDEC {$$ = new iCPostfixExpression($1, *$2, *parser_context); delete $2;} // ++
			 ;

//argument list for function call - list of comma-separated assignment expressions
//can optionaly be terminated by an extra comma (C legacy)
arg_expr_list  :	arg_expr_list TCOMMA assignment_expr
					{
						if(NULL != $3)
						   $1->push_back($3); 
						$$=$1;
						delete $2;
					}
			   |	assignment_expr
					{
						$$ = new std::vector<iCExpression*>; 
						if(NULL != $1)
						   $$->push_back($1);
					}
			   ;
//iCExpression*
primary_expr : TTRUE   {$$ = new iCLogicConst(true, *parser_context); $1;}
			 | TFALSE  {$$ = new iCLogicConst(false, *parser_context); $1;}
			 | TICONST {$$ = new iCInteger(*$1, *parser_context); delete $1;}
			 | TDCONST {$$ = new iCDouble(*$1, *parser_context); delete $1;}
			 | THCONST {$$ = new iCInteger(*$1, *parser_context); delete $1;}
			 | TBCONST {$$ = new iCInteger(*$1, *parser_context); delete $1;}
			 | TIDENTIFIER // can either be a variable or an mcu declaration
				{
					const iCScope* scope = parser_context->get_mcu_decl_scope(*$1);
					if(NULL != scope) // mcu declaration
					{
						$$ = new iCMCUIdentifier(*$1, *parser_context);
					}
					else // variable or proctype param
					{
						iCVariable* var = parser_context->get_var(*$1);
						if(NULL == var) // proctype param or undeclared variable
						{
							const iCProcType* proctype = parser_context->get_proctype();
							if (NULL != proctype)
							{
								bool id_is_proctype_param = false;
								iCProcTypeParamList params_list = proctype->get_params();
								for (iCProcTypeParamList::iterator i = params_list.begin(); i != params_list.end(); i++)
								{
									if (0 == (*i)->name.compare(*$1))
									{
										//todo: fill scope
										$$ = new iCProcTypeParamUsage(*$1, *i, NULL, *parser_context);
										id_is_proctype_param = true;
										break;
									}
								}
								if (!id_is_proctype_param)
								{
									parser_context->err_msg("id is not proctype param, %s was not declared in this scope", $1->c_str());
									$$ = new iCIdentifier(*$1, NULL, *parser_context);
								}
							}
							else //NULL == proctype
							{
								parser_context->err_msg("%s was not declared in this scope", $1->c_str());
								$$ = new iCIdentifier(*$1, NULL, *parser_context);
							}
						}
						else //NULL != var
						{
							const iCProcess* proc = parser_context->get_process();
							if(NULL != proc)//added because of functions and proctypes - vars inside them don't belong to any proc
							{
								if(0 != proc->activator.compare("background"))
								{
									//Mark var as used in ISR - used for volatile checks
									var->used_in_isr = true;
									parser_context->add_to_second_pass(var);
								}
								$$ = new iCIdentifier(*$1, var->scope, *parser_context);
							}
							else
							{
								const iCProcType* proctype = parser_context->get_proctype();
								if (NULL != proctype) 
								{
									std::cout << "parser.y proctype is not null" << std::endl;

									bool var_belongs_to_proctype = false; //var is inside proctype but it's not state local var
									iCVariablesList var_list = proctype->get_variables();
									for (iCVariablesList::iterator i = var_list.begin(); i != var_list.end(); i++)
									{
										if (0 == (*i)->name.compare(*$1))
										{
											std::cout << "var " << *$1 << " in proctype and out state" << std::endl;
											$$ = new iCIdentifierInProcType(*$1, var->scope, *parser_context);
											var_belongs_to_proctype = true;
											break;
										}
									}
									if (!var_belongs_to_proctype)
									{
										std::cout << "var "<< *$1 <<" in proctype and in state" << std::endl;
										$$ = new iCIdentifier(*$1, var->scope, *parser_context);
									}
								}
								else
								{
									$$ = new iCIdentifier(*$1, var->scope, *parser_context);
								}
							}
						}
					}
					delete $1;
				}
			 |	TLPAREN expr TRPAREN {$$ = new iCPrimaryExpression($2, *parser_context);$1;$3;}
			 |	TIDENTIFIER TACTIVE
				{
					//DOT graph test
					//const iCProcess* proc = parser_context->get_process();
					//add edge to the process graph (for DOT generation)
					//proc_graph.insert(ProcGraphNode(proc->name, *$1));

					$$ = new iCProcStatusCheck(*$1, true, *parser_context);
					delete $1;
					parser_context->add_to_second_pass($$);
					$2;//suppress unused value warning
				}
			 |	TIDENTIFIER TPASSIVE 
				{
					//DOT graph test
					//const iCProcess* proc = parser_context->get_process();
					//add edge to the process graph (for DOT generation)
					//proc_graph.insert(ProcGraphNode(proc->name, *$1));

					$$ = new iCProcStatusCheck(*$1, false, *parser_context);
					delete $1; 
					parser_context->add_to_second_pass($$);
					$2;//suppress unused value warning
				}
			 |	TCCODEEXPR 
				{
					$$ = new CCodeLine(*$1, *parser_context);
					delete $1;
				}
				//TODO: add hyperprocess active/inactive checks
			 |	TSTRING
				{
					$$ = new iCString(*$1, *parser_context);
					delete $1;
				}
			 ;
//std::string*
assignement_op : TASSGN 
			   | TR_ASSGN  		
			   | TL_ASSGN 		
			   | TPLUS_ASSGN  	
			   | TMINUS_ASSGN  	
			   | TASTERISK_ASSGN  
			   | TDIV_ASSGN	 
			   | TPERC_ASSGN  
			   | TAND_ASSGN	 
			   | TXOR_ASSGN	 
			   | TOR_ASSGN	 
			   ;

//=============================================================================
//cast expression's a bitch
//for now we just shove it all in a string, call it type_name
//and let the c compiler deal with it
//=============================================================================
//iCExpression*
cast_expr 	: unary_expr 
			| TLPAREN type_name TRPAREN cast_expr 
			  {
				  $$ = new iCCastExpression(*$2, $4, *parser_context); 
				  delete $2; 
				  $1; $3; 
			  }
		  	;
type_name	:	decl_specs 
				{
					$$ = new std::string;
					for(iCStringList::iterator i=$1->begin();i!=$1->end();i++)
						*$$ += " " + *i;
					delete $1;
				}
			|	decl_specs abstract_declarator 
			{
				$$ = new std::string;
				for(iCStringList::iterator i=$1->begin();i!=$1->end();i++)
						*$$ += " " + *i;
				*$$ += " " + *$2; 
				delete $1;
				delete $2; 
			}
			;

abstract_declarator	:	pointer 
					|	direct_abstract_declarator
					|	pointer direct_abstract_declarator 
						{
							$$ = $1; 
							*$$ += " " + *$2; 
							delete $2; 
						}
					;

direct_abstract_declarator	:	TLPAREN abstract_declarator TRPAREN
								{
									$$ = new std::string;
									*$$ += '(' + *$2 + ')'; 
									delete $2;
									$1;$3;
								}
							|	TLBRACKET TRBRACKET
								{
									$$ = new std::string("[]");
									$1;$2;
								}
							|	TLBRACKET TICONST TRBRACKET // should be binary expression instead of TICONST
								{
									$$ = new std::string;
									*$$ += '[' + *$2 + ']';
									delete $2;
									$1;$3;
								}
							|	direct_abstract_declarator TLBRACKET TRBRACKET
								{
									$$ = $1;
									*$$ += "[]";
									$2; $3;
								}
							|	direct_abstract_declarator TLBRACKET TICONST TRBRACKET // should be binary expression instead of TICONST
								{
									$$ = $1;
									*$$ += '[' + *$3 + ']';
									delete $3;
									$2; $4;
								}
							//|	'(' ')'
							//|	'(' parameter_type_list ')'
							//|	direct_abstract_declarator '(' ')'
							//|	direct_abstract_declarator '(' parameter_type_list ')'
							;

pointer	:	TASTERISK 
			{
				$$ = new std::string("*"); 
				delete $1; 
			}
		//|	'*' type_qualifier_list
		|	TASTERISK pointer 
			{
				$$ = $2; 
				*$$ += "*"; 
				delete $1;  
			}
		//|	'*' type_qualifier_list pointer
		;

/*************************************************************************************************/     
/*                              D E C L A R A T I O N S                                          */     
/*************************************************************************************************/

//=============================================================================
//Function Definition
//Parameters are put in a preopened scope func_params_xx
//Function body (a compound statement) opens its own scope, nested within
//the func_param_xx scope (scope names do not add up)
//TODO: Func declaration (prototype) is treated as a definition
//Need to allow declarations and definitions with redefinition (body != NULL)
//and undefined (body == NULL on second pass) checks
//=============================================================================
func_definition			:	decl_specs func_declarator
							{
								parser_context->set_func($<func>2);//entering function
								$<func>$ = NULL;
							}
							func_body // compound statement
							{
								$$ = $2;//func declarator
								$$->set_type_specs(*$1);
								$$->body = $4;//func_body	
								delete $1;//decl_specs

								//if there were params, they had a scope opened (in func_declarator midrule action)
								//that needs to be closed
								if(!$$->params.empty())
									parser_context->close_scope();

								parser_context->set_func(NULL);//leaving function
							}
						;

func_body				:	compound_statement { $$ = $1; }
						|	TSEMIC { $$ = NULL; $1; }
						;

func_declarator			: TIDENTIFIER TLPAREN TRPAREN //function with empty parentheses - no params
						  {
							  parser_context->check_identifier_defined(*$1);

							  //Create the function object and add it to scope
							  const iCScope* scope = parser_context->get_current_scope();
							  $$ = new iCFunction(*$1, scope, *parser_context);
							  parser_context->add_func_to_scope(*$1);
							  delete $1;

							  $2;$3;
						  }
						| TIDENTIFIER TLPAREN // 1 2
						  {
							  parser_context->open_scope("func_params");
						  }
						  param_list TRPAREN // 4 5 function name with params list in parentheses
						  {
							  parser_context->check_identifier_defined(*$1);
							
							  //Create the function object, add it to scope and set its params
							  const iCScope* scope = parser_context->get_current_scope();
							  $$ = new iCFunction(*$1, scope, *parser_context);
							  parser_context->add_func_to_scope(*$1);
							  $$->set_params(*$4);
							  delete $1;
							  delete $4;
							  $2;$5;
						  }
						;

param_list				: param_list TCOMMA param_declarator 
						  {
							  $$ = $1;
							  $$->push_back($3);
							  delete $2;
						  }
						| param_declarator 
						{
							$$ = new iCVariablesList;
							$$->push_back($1);
						}
						;

param_declarator		: decl_specs direct_declarator 
						  {
							  $$ = $2;
							  $$->set_type_specs(*$1);
							  delete $1;
						  }
						;

//=============================================================================
//Variable Declaration
//=============================================================================
var_declaration			:	decl_specs init_declarator_list TSEMIC 
							{
								$$ = $2;
								for(iCVariablesList::iterator i=$2->begin();i!=$2->end();i++)
								{
									iCVariable* var = *i;
									var->set_type_specs(*$1);
								}
								delete $1;
								$3;//suppress unused value warning
							}
						;

decl_specs				:	decl_specs type_spec {$1->push_back(*$2); delete $2; $$=$1;}
						|	type_spec {$$ = new iCStringList; $$->push_back(*$1); delete $1;}
						;

init_declarator_list	:	init_declarator_list TCOMMA init_declarator 
							{
								$$=$1;
								$1->push_back($3);
								delete $2;
							}
						|	init_declarator 
							{
								$$ = new iCVariablesList;
								$$->push_back($1);
							}
						;

init_declarator			:	direct_declarator {$$ = $1;}
						|	direct_declarator TASSGN initializer 
							{
								$$ = $1;
								$$->set_initializer($3);
								delete $2;
							}
						;

//can be just as simple as a variable name
//or an array name with dimensions in brackets, e.g. matrix[3][10]
//in C these can get really complex, with pointers, function pointers etc.
direct_declarator		:	TIDENTIFIER 
						{
							parser_context->check_identifier_defined(*$1);

							const iCScope* scope = parser_context->get_current_scope();
							$$ = new iCVariable(*$1, scope, *parser_context);
							parser_context->add_var_to_scope($$);
							delete $1;
						}
						| direct_declarator TLBRACKET binary_expr TRBRACKET //explicit array dimension
						{
							$$ = $1;
							$$->add_dimension($3);
							$2;$4;
						}
						| direct_declarator TLBRACKET TRBRACKET //implicit array dimension (size will be specified by initializer)
						{
							$$ = $1;
							$$->add_dimension(NULL); //dimension size is implicit
							$2;$3;
						}
						;

//initializers form a tree in case of multi-dimensional arrays
//array initializer list can be optionally terminated with an extra comma (C legacy)
initializer 			:	assignment_expr  //simple variable initializer
							{
								$$ = new iCInitializer(*parser_context);
								$$->add_initializer($1);
							}
						|	TLBRACE initializer_list TRBRACE //array initializer
							{
								$$ = $2;
								$1;$3;
							}
						|	TLBRACE initializer_list TCOMMA TRBRACE //array initializer with and extra comma at the end
							{
								$$ = $2;
								delete $3;
								$1;$4;
							}
						;
initializer_list		:	initializer_list TCOMMA initializer 
							{
								$$ = $1;
								$$->add_initializer($3);
								delete $2;
							}
						|	initializer 
							{
								$$ = $1;
							}
						;

type_spec				:	TVOID	
						|	TCHAR	
						|	TINT		
						|	TSHORT	
						|	TLONG	
						|	TFLT		
						|	TDBL		
						|	TSGND	
						|	TUNSGND	
						|	TBOOL
						|	TCONST
						|	TVOLATILE	
						|	TINLINE		
						;

mcu_declaration			:	TVECTOR		TIDENTIFIER TSEMIC {$$ = $2;$1;$3;}
						|	TREGISTER	TIDENTIFIER TSEMIC {$$ = $2;$1;$3;}
						|	TBIT		TIDENTIFIER TSEMIC {$$ = $2;$1;$3;}
						;

%%

int ic_error(const char *s) 
{
	parser_context->err_msg(s);
	return 0;
}
