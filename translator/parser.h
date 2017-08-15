
%{
	#include "iCProgram.h"
	#include "iCScope.h" 
	#include "ParserContext.h"
	#include "iCIdentifier.h"
	#include "iCMCUIdentifier.h"
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
	//#include "iCVariableDeclaration.h"
	#include "iCVariable.h"
	#include "iCFunction.h"
	#include "iCFunctionCall.h"

	#include <stdio.h>
	#include <stdarg.h> 
	#include <typeinfo>
	#include <set>
	

    iCProgram* ic_program = NULL; /* AST root */
    //extern iCScope* current_scope;
	extern ParserContext* parser_context;

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
    iCProcess* process;
    iCState* state;
    iCStatement* statement;
    CCode* ccode;
    NodesList* nodes_list;
    StateList* state_list;
    iCBlockItemsList* block_items_list;
	iCBlockItem* block_item;
	iCTimeout* timeout;
    iCExpression* expression;
    std::string *string;
    iCStringList *str_list;
    int token;
	std::list<iCVariable*>* var_list;
    
	iCHyperprocess* hyperprocess;

    iCDeclarationList* decl_list;
    iCProcessList* proc_list;
    iCDeclaration* declaration;
	iCVariable* variable;
    iCIdentifierList* ident_list;

	iCFunction* func;
	std::vector<iCExpression*>* expr_list;

	iCProgramItemsList* program_items_list;
	iCProgramItem* program_item;

	iCProcBody* proc_body;
}

/***********************************************/
/*                   TOKENS                    */
/***********************************************/
%token <token> TPROC			"process"
%token <token> TSTATE			"state"
%token <token> TTO				"to"
%token <token> TSTART			"start"
%token <token> TSTOP			"stop"
%token <token> TTIMEOUT			"timeout"
%token <token> TACTIVE			"active"
%token <token> TPASSIVE			"passive"
%token <token> TSIZEOF			"sizeof"
%token <token> THYPERPROCESS	"hyperprocess"
%token <token> TVECTOR			"vector"	
%token <token> TREGISTER		"register"		
%token <token> TBIT				"bit"		
%token <token> TENABLED			"enabled"	
%token <token> TSET				"set"		
%token <token> TCLEAN			"clean"	
%token <token> TIF				"if"
%token <token> TELSE			"else"
%token <token> TFOR				"for"
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
%token <string> TIDENTIFIER		"identifier"
%token <string> TICONST			"integer constant"
%token <string> TDCONST			"double constant"
%token <string> THCONST			"hex constant"
%token <string> TBCONST			"binary constant"
%token <string> TCCODELINE		"c code line"
%token <string> TCCODEEXPR		"c code expression"
%token <string> TSTRING			"string literal"
%token <token> TLPAREN			"(" 
%token <token> TRPAREN			")" 
%token <token> TLBRCKT			"["	
%token <token> TRBRCKT			"]"	
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
%token <string> TMUL			"*" 
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
%token <string> TMUL_ASSGN		"*="		
%token <string> TDIV_ASSGN		"/="		
%token <string> TPERC_ASSGN		"%="	
%token <string> TAND_ASSGN		"&="		
%token <string> TXOR_ASSGN		"^="		
%token <string> TOR_ASSGN		"|="	


/***********************************************/
/*                   NODES                     */
/***********************************************/
%type <hyperprocess>		hp_definition	
%type <process>				proc_def
%type <proc_body>			proc_body
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
%type <statement>			prep_compound
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
%type <str_list>			init_declarator_list		
//%type <string>				init_declarator		
%type <string>				direct_declarator					
//%type <string>				initializer 		
%type <string>				type_spec		
%type <func>				func_definition
%type <expr_list>			arg_expr_list


/***********************************************/
/*                  DESTRUCTORS                */
/***********************************************/

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
	for(StateList::iterator i=$$->states.begin();i!=$$->states.end();i++)
	{
		delete *i;
	}
	for(iCDeclarationList::iterator i=$$->decls.begin();i!=$$->decls.end();i++)
	{
		delete *i;
	}

	delete $$;
}<proc_body> 

/*%destructor
{
	for(std::vector<iCExpression*>::iterator i=$$->begin();i!=$$->end();i++)
	{
		delete *i;
	}
	delete $$;
}<expr_list>
*/

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

%left TLOR
%left TLAND
%left TOR
%left TXOR
%left TAND
%left TEQ TNEQ
%left TGT TLT TGE TLE
%left TLSHIFT TRSHIFT
%left TPLUS TMINUS
%left TMUL TDIV TPERC

%left TINC TDEC UMINUS TTILDE TEXCLAM




/***********************************************/
/*                   GRAMMAR                   */
/***********************************************/

%%

program		:	{
					ic_program = new iCProgram(*parser_context);
					ic_program->add_hyperprocess(new iCHyperprocess("background", *parser_context));
					parser_context->set_program(ic_program);
					parser_context->add_to_second_pass(ic_program);
				}
				program_items_list 
				{
					parser_context->close_scope();
				}
			;

program_items_list	:	program_items_list program_item 
					|	program_item 
					;

program_item	:	var_declaration	
				{
					for(std::list<iCVariable*>::iterator i=$1->begin();i!=$1->end();i++)
					{
						ic_program->add_variable(*i);
					}
					delete $1;
					//$$ = NULL;
					//if(NULL!=$1)ic_program->add_declaration($1);
				}
				|	mcu_declaration	
				{
					//This code duplicated in direct_declarator - either add a check_scope function or use direct declarator for mcu definitions
					//check if already defined in this scope
					const iCScope* var_scope = parser_context->get_var_scope(*$1);
					const iCScope* mcu_decl_scope = parser_context->get_mcu_decl_scope(*$1);
					const iCScope* func_scope = parser_context->get_func_scope(*$1);
					if(NULL != var_scope || NULL != mcu_decl_scope || NULL != func_scope)
					{
						const iCScope* scope = (NULL != var_scope)?var_scope:((NULL != mcu_decl_scope)?mcu_decl_scope:func_scope);
						parser_context->err_msg("symbol redefinition: %s aready defined in %s",
							$1->c_str(), scope->name.empty()?"this scope":scope->name.c_str());
					}

					//no need to generate the declaration object
					//just add it to scope
					parser_context->add_mcu_decl_to_scope(*$1); 
					delete $1;	
				}
				|	proc_def		{if(NULL!=$1)ic_program->add_process($1);		}
				|	hp_definition	{if(NULL!=$1)ic_program->add_hyperprocess($1);	}
				|	c_code			{if(NULL!=$1)ic_program->add_mcu_declaration($1);	}
				/*|	TCCODEEXPR 
				{
					//$$ = new CCodeLine(*$1, *parser_context);
					//delete $1;
				}*/
				|	func_definition		
					{
						ic_program->add_function($1);
					}
				;

c_code		:	TCCODELINE 
				{
					$$ = new CCode(*parser_context); 
					$$->add_line(new CCodeLine(*$1, *parser_context)); 
					delete $1;
				}
			/*|	TCCODEEXPR*/
			;


hp_definition	:	THYPERPROCESS TIDENTIFIER // 1  2
					{
						//check hyperprocess redefinition
						if(ic_program->hp_defined(*$2))
						{
							parser_context->err_msg("hyperprocess redefinition: %s already defined", $2->c_str());
						}
						$<hyperprocess>$ = new iCHyperprocess(*$2, *parser_context);
						delete $2;
					}
					TLBRACE // 4
					TVECTOR		TASSGN TIDENTIFIER TSEMIC // 5 6 7 8
					TREGISTER	TASSGN TIDENTIFIER TSEMIC // 9 10 11 12
					TBIT		TASSGN TIDENTIFIER TSEMIC // 13 14 15 16
					TRBRACE	// 17
					{
						if(NULL == parser_context->get_mcu_decl_scope(*$7)){
							parser_context->err_msg("undefined interrupt vector: %s ", $7->c_str());
						}
						if(NULL == parser_context->get_mcu_decl_scope(*$11)){
							parser_context->err_msg("undefined mcu register: %s ", $11->c_str());
						}
						if(NULL == parser_context->get_mcu_decl_scope(*$15)){
							parser_context->err_msg("undefined register bit: %s ", $15->c_str());
						}

						$$ = $<hyperprocess>3;
						$$->set_params(*$7, *$11, *$15);

						//params
						delete $7;
						delete $11;
						delete $15;

						//TASSGN
						delete $6;
						delete $10;
						delete $14;
						$1;$4;$5;$8;$9;$12;$13;$16;$17;//suppress unused value warning
					}
				;

proc_def	:	TPROC TIDENTIFIER // 1 2
				{
					//check for process redefinition
					const iCScope* scope = parser_context->get_proc_scope(*$2);
					if(NULL != scope)
					{
						parser_context->open_scope(*$2);
						parser_context->err_msg("process redefinition: %s already defined in %s",
												$2->c_str(), scope->name.empty()?"this scope":scope->name.c_str());
						$<process>$ = NULL;
					}
					else
					{
						$<process>$ = new iCProcess(*$2, *parser_context);
						parser_context->set_process($<process>$);
						parser_context->open_scope(*$2);
					}
					delete $2;
				}
				TCOLON TIDENTIFIER // 4 5
				{
					if(!ic_program->hp_defined(*$5))
					{
						parser_context->err_msg("undefined hyperprocess: %s", $5->c_str());
					}
					if(NULL != $<process>3)
					{
						$<process>3->set_hp(*$5);
					}
				}
				TLBRACE proc_body TRBRACE // 7 8 9 
				{
					parser_context->close_scope();

					//process redefiniton
					if(NULL == $<process>3)
					{
						//delete proc_body
						for(StateList::iterator i=$8->states.begin();i!=$8->states.end();i++)
							delete *i;
						for(iCDeclarationList::iterator i=$8->decls.begin();i!=$8->decls.end();i++)
							delete *i;

						delete $5;
						delete $8;
						$$ = NULL;
					}
					else
					{
						
						$$ = $<process>3;
						$$->add_states($8->states);
						//$$->add_decls($8->decls);
						parser_context->add_proc_to_scope($$->name);
						
						delete $5;
						delete $8;
					}
					$1;$4;$7;$9;//suppress unused value warning
				}
			;	

		
proc_body	: proc_body state {$1->states.push_back($2); $$=$1;}
			| proc_body var_declaration 
			{
				std::list<iCVariable*>* vars = $2;
				for(std::list<iCVariable*>::iterator i=vars->begin();i!=vars->end();i++)
				{
					ic_program->add_variable(*i);
				}
				delete vars;
				$$ = $1;
				//$1->decls.push_back($2); $$=$1;
			}
			| state {$$ = new iCProcBody(); $$->states.push_back($1); }
			| var_declaration 
			{
				std::list<iCVariable*>* vars = $1;
				for(std::list<iCVariable*>::iterator i=vars->begin();i!=vars->end();i++)
				{
					ic_program->add_variable(*i);
				}
				delete vars;
				//$$ = NULL;
				$$ = new iCProcBody(); //$$->decls.push_back($1);
			}
			| c_code proc_body{if(NULL!=$1)$2->decls.push_back($1);	$$=$2;}

			;

state		:	TSTATE TIDENTIFIER
				{
					//check for state redefinition
					const iCScope* scope = parser_context->get_state_scope(*$2);
					if(NULL != scope)
					{
						parser_context->err_msg("state redefinition: %s already defined in %s",
												$2->c_str(), scope->name.empty()?"this scope":scope->name.c_str());
					}
					parser_context->add_state_to_scope(*$2);
					parser_context->open_scope(*$2); 
					$<state>$ = new iCState(*$2, *parser_context);
					parser_context->set_state($<state>$);
					delete $2;
				}
				TLBRACE state_body TRBRACE 
				{
					$$ = $<state>3; 
					if(NULL != $5)
						$$->set_items(*$5); 
					parser_context->close_scope(); 
					delete $5;
					$1;$2;$4;$6;//suppress unused value warning
				}
			;
		
state_body	:	%empty {$$ = new iCBlockItemsList();}
			|	state_items_list {$$ = $1;}
			;

state_items_list	:	state_items_list state_block_item 
						{
							if(NULL != $2)
							{
								$1->push_back($2);
							}
							$$=$1;
						}
					|	state_block_item 
					{
						$$ = new iCBlockItemsList(); 
						if(NULL != $1)
							$$->push_back($1);
					}
					|	state_items_list error TSEMIC 
					{
						yyerrok;
						$$=$1;
						$3;
						
					}
					|	error TSEMIC
						{
							$$ = new iCBlockItemsList();
							yyerrok;
							$2;
						}
					;

state_block_item	:	block_item	{$$ = $1;}
					|	timeout {$$ = $<block_item>1;}
					;

block_item	:	var_declaration 
				{
					std::list<iCVariable*>* vars = $1;
					for(std::list<iCVariable*>::iterator i=vars->begin();i!=vars->end();i++)
					{
						ic_program->add_variable(*i);
					}
					delete vars;
					$$ = NULL;
					//$$ = $<block_item>1;
				}
			|	statement {$$ = $<block_item>1;}
			|	c_code {$$ = $<block_item>1;}	
			;
			
statement	:	TTO TSTATE TIDENTIFIER TSEMIC //state transition
				{
					const iCProcess* proc = parser_context->get_process();
					if(NULL == proc)
					{
						parser_context->err_msg("state transitions can only be used inside states");
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
			|	TSTART TPROC TIDENTIFIER TSEMIC //start process
				{
					const iCProcess* proc = parser_context->get_process();
					if(NULL == proc)
					{
						parser_context->err_msg("start process statement can only be used inside states");
						$$ = NULL;
					}
					else
					{
						$$ = new iCStartProcStatement(*$3, *parser_context); 
						parser_context->add_to_second_pass($$); // to check if process was defined
					}
					
					delete $3;
					$1;$2;$4;//suppress unused value warning
				}	
			|	TSTOP TPROC TIDENTIFIER TSEMIC //stop process
				{
					const iCProcess* proc = parser_context->get_process();
					if(NULL == proc)
					{
						parser_context->err_msg("stop process statement can only be used inside states");
						$$ = NULL;
					}
					else
					{
						$$ = new iCStopProcStatement(*$3, *parser_context); 
						parser_context->add_to_second_pass($$); // to check if process was defined
					}
					
					delete $3;
					$1;$2;$4;//suppress unused value warning
				}	
			|	TSTOP TPROC TSEMIC
				{
					const iCProcess* proc = parser_context->get_process();
					if(NULL == proc)
					{
						parser_context->err_msg("stop process statement can only be used inside states");
						$$ = NULL;
					}
					else
					{
						$$ = new iCStopProcStatement(proc->name, *parser_context); 
						parser_context->add_to_second_pass($$);
					}
					
					$1;$2;$3;//suppress unused value warning
				}
			|	expr TSEMIC //expression statement
				{
					$$ = new iCExpressionStatement($1, *parser_context);
					$$->line_num = parser_context->line();
					$2;//suppress unused value warning
				}					
			|	TSTART THYPERPROCESS TIDENTIFIER TSEMIC //hyperprocess control - start
				{
					$$ = new iCStartHPStatement(*$3, *parser_context); 
					parser_context->add_to_second_pass($$); // to check if HP was defined
					delete $3;
					$1;$2;$4;//suppress unused value warning
				}
			|	TSTOP THYPERPROCESS TIDENTIFIER TSEMIC //hyperprocess control - stop
				{
					$$ = new iCStopHPStatement(*$3, *parser_context); 
					parser_context->add_to_second_pass($$); // to check if HP was defined
					delete $3;
					$1;$2;$4;//suppress unused value warning
				}
			|	TSTOP THYPERPROCESS TSEMIC
				{
					const iCProcess* proc = parser_context->get_process();
					if(NULL == proc)
					{
						parser_context->err_msg("stop hyperprocess statement can only be used inside states");
						$$ = NULL;
					}
					else
					{
						$$ = new iCStopHPStatement(proc->activator, *parser_context); 
						parser_context->add_to_second_pass($$); // required for it to work
					}
					
					$1;$2;$3;//suppress unused value warning
				}
			|	compound_statement
				{
					$$ = $1;
				}
			|	TIF TLPAREN expr TRPAREN statement TELSE statement
				{
					$<statement>$ = new iCSelectionStatement(*parser_context);
					static_cast<iCSelectionStatement*>($<statement>$)->body = $5;
					static_cast<iCSelectionStatement*>($<statement>$)->else_body = $7;
					static_cast<iCSelectionStatement*>($<statement>$)->set_expression($3);

					$1;$2;$4;$6;$7;//suppress unused value warning
				}
			|	TIF TLPAREN expr TRPAREN statement %prec XIF
				{
					$<statement>$ = new iCSelectionStatement(*parser_context);
					static_cast<iCSelectionStatement*>($<statement>$)->body = $5;
					static_cast<iCSelectionStatement*>($<statement>$)->set_expression($3);
					
					$1;$2;$4;//suppress unused value warning
				}
			
			;

compound_statement:		prep_compound TLBRACE block_items_list TRBRACE // compound statement
						{
							$$ = $1;
							if(NULL != $3)
							{
								static_cast<iCCompoundStatement*>($<statement>$)->set_items(*$3);
								delete $3;
							}

							parser_context->close_scope();
							$2;$4;//suppress unused value warning
						}
					|	prep_compound TLBRACE TRBRACE // empty compound statement {}
						{
							$$ = $1;
							parser_context->close_scope();
							$2;$3;
						}
					;

prep_compound:	%empty // subroutine to prepare scope for compound statement
				{
					$<statement>$ = new iCCompoundStatement(*parser_context);
					parser_context->open_scope("comp");
				}

timeout	:	TTIMEOUT TLPAREN TICONST TRPAREN
			{
			  $<timeout>$ = new iCTimeout(atoi($3->c_str()), *parser_context);
			  parser_context->open_scope("timeout");
			  delete $3;
			}
			TLBRACE block_items_list TRBRACE
			{
			  $$ = $<timeout>5;
			  parser_context->close_scope();
			  $$->set_items(*$7);
			  delete $7;
			  $1;$2;$4;$6;$8;//suppress unused value warning
			}
		;

block_items_list	:	block_items_list block_item 
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
			

/***********************************************/     
/*           E X P R E S S I O N S             */     
/***********************************************/

expr 		: assignment_expr 
	 		;
	 
assignment_expr : binary_expr 
				| unary_expr assignement_op assignment_expr {$$ = new iCAssignmentExpression($1, *$2, $3, *parser_context); delete $2;}
				;
				
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
			| binary_expr	 TMUL		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TDIV		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			| binary_expr	 TPERC		binary_expr {$$ = new iCBinaryExpression($1, *$2, $3, *parser_context); delete $2;}
			
			;
			
cast_expr 	: unary_expr 
		  	/*| TLPAREN type_name TRPAREN cast_expr*/
		  	;
		  
unary_expr 	: postfix_expr 
		   	| TINC unary_expr {$$ = new iCUnaryExpression(*$1, $2, *parser_context); delete $1;}
		   	| TDEC unary_expr {$$ = new iCUnaryExpression(*$1, $2, *parser_context); delete $1;}
		   	| TMINUS cast_expr %prec UMINUS {$$ = new iCUnaryExpression(*$1, $2, *parser_context); delete $1;}
			| TTILDE cast_expr {$$ = new iCUnaryExpression(*$1, $2, *parser_context); delete $1;}
			| TEXCLAM cast_expr {$$ = new iCUnaryExpression(*$1, $2, *parser_context); delete $1;}
		   	/*| TSIZEOF unary_expr {$$ = new iCUnaryExpression($1, $2);}*/
		   	/*| TSIZEOF type_name {$$ = new iCUnaryExpression($1, $2);}*/
		   	;
		   	
postfix_expr : primary_expr 
			 /*| postfix_expr TLBRCKT expr TRBRCKT*/
			 |	TIDENTIFIER TLPAREN TRPAREN 
			   {
				   $$ = new iCFunctionCall(*$1, *parser_context);
				   delete $1;
				   $2;$3;
			   }
			 |	 TIDENTIFIER TLPAREN arg_expr_list TRPAREN
				 {
					 iCFunctionCall* func_call = new iCFunctionCall(*$1, *parser_context);
					 func_call->set_args(*$3);
					 $$ = func_call;
					 /*for(std::vector<iCExpression*>::iterator i=$3->begin();i!=$3->end();i++)
						 delete *i;*/
					 delete $1;
					 delete $3;
					 $2;$4;
				 }
			 | postfix_expr TINC {$$ = new iCPostfixExpression($1, *$2, *parser_context); delete $2;}
			 | postfix_expr TDEC {$$ = new iCPostfixExpression($1, *$2, *parser_context); delete $2;}
			 ;
			 
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

primary_expr : TTRUE   {$$ = new iCLogicConst(true, *parser_context); $1;}
			 | TFALSE  {$$ = new iCLogicConst(false, *parser_context); $1;}
			 | TICONST {$$ = new iCInteger(*$1, *parser_context); delete $1;}
			 | TDCONST {$$ = new iCDouble(*$1, *parser_context); delete $1;}
			 | THCONST {$$ = new iCInteger(*$1, *parser_context); delete $1;}
			 | TBCONST {$$ = new iCInteger(*$1, *parser_context); delete $1;}
			 |	TIDENTIFIER // can either be a variable or an mcu declaration
				{
					const iCScope* scope = parser_context->get_mcu_decl_scope(*$1);
					if(NULL != scope) // mcu declaration exists
					{
						$$ = new iCMCUIdentifier(*$1, *parser_context);
					}
					else
					{
						iCVariable* var = parser_context->get_var(*$1);
						
						if(NULL == var) // variable wasn't declared
						{
							parser_context->err_msg("%s was not declared in this scope", $1->c_str());
							$$ = new iCIdentifier(*$1, NULL, *parser_context);
						}
						else
						{
							$$ = new iCIdentifier(*$1, var->scope, *parser_context);
							const iCProcess* proc = parser_context->get_process();
							if(NULL != proc)//added because of functions - vars in functions don't belong to proc
								if(0 != proc->activator.compare("background"))
								{
									var->used_in_isr = true;
								}
						}
					}
					delete $1;
				}
			 |	TLPAREN expr TRPAREN {$$ = new iCPrimaryExpression($2, *parser_context);$1;$3;}
			 |	TIDENTIFIER TACTIVE
				{
					$$ = new iCProcStatusCheck(*$1, true, *parser_context);
					delete $1;
					parser_context->add_to_second_pass($$);
					$2;//suppress unused value warning
				}
			 |	TIDENTIFIER TPASSIVE 
				{
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
				/* add hyperprocess control - check*/
			 ;

assignement_op : TASSGN 
			   | TR_ASSGN  		
			   | TL_ASSGN 		
			   | TPLUS_ASSGN  	
			   | TMINUS_ASSGN  	
			   | TMUL_ASSGN  
			   | TDIV_ASSGN	 
			   | TPERC_ASSGN  
			   | TAND_ASSGN	 
			   | TXOR_ASSGN	 
			   | TOR_ASSGN	 
			   ;
			 
/*unary_op : TMINUS 
		 | TTILDE 
		 | TEXCLAM
		 ;*/

/***********************************************/     
/*           D E C L A R A T I O N S           */     
/***********************************************/

func_definition			:	decl_specs direct_declarator TLPAREN  
							{
								const iCScope* scope = parser_context->get_current_scope();
								$<func>$ = new iCFunction(*$1, *$2, scope, *parser_context);
								parser_context->add_func_to_scope(*$2);
							}
							TRPAREN compound_statement // compound statement
							{
								$$ = $<func>4;
								$$->body = $6;	
								delete $1;
								delete $2;
								$3;$5;
							}
						/*|
							decl_specs direct_declarator TLPAREN TRPAREN TSEMIC
							{
								const iCScope* scope = parser_context->get_current_scope();
								$$ = new iCFunction(*$1, *$2, scope, *parser_context);
								parser_context->add_func_to_scope(*$2);
								delete $1;
								delete $2;
								$3;$4;$5;
							}*/
						;


var_declaration			:	decl_specs init_declarator_list TSEMIC 
							{
								const iCScope* scope = parser_context->get_current_scope();
								$$ = new std::list<iCVariable*>;
								for(iCStringList::iterator i=$2->begin();i!=$2->end();i++)
								{
									iCVariable* var = new iCVariable(*$1, *i, scope, *parser_context);
									$$->push_back(var);
									parser_context->add_var_to_scope(var);
								}
								delete $1;
								delete $2;
								
								$3;//suppress unused value warning*/
							}
							;

mcu_declaration			:		TVECTOR		TIDENTIFIER TSEMIC {$$ = $2;$1;$3;}
							|	TREGISTER	TIDENTIFIER TSEMIC {$$ = $2;$1;$3;}
							|	TBIT		TIDENTIFIER TSEMIC {$$ = $2;$1;$3;}
							;

decl_specs				:	decl_specs type_spec {$1->push_back(*$2); delete $2; $$=$1;}
						|	type_spec {$$ = new iCStringList; $$->push_back(*$1); delete $1;}
						;

init_declarator_list	:	init_declarator_list TCOMMA direct_declarator {$1->push_back(*$3); delete $3; delete $2; $$=$1;}
						|	direct_declarator {$$ = new iCStringList; $$->push_back(*$1); delete $1;}
						;

/*init_declarator			:	declarator {$$ = $1;}
							|	declarator TASSGN initializer {$$ = $1; delete $2; delete $3;}
							;*/

direct_declarator				:	TIDENTIFIER 
							{
								//check if already defined in this scope
								const iCScope* var_scope = parser_context->get_var_scope(*$1);
								const iCScope* mcu_decl_scope = parser_context->get_mcu_decl_scope(*$1);
								const iCScope* func_scope = parser_context->get_func_scope(*$1);
								if(NULL != var_scope || NULL != mcu_decl_scope || NULL != func_scope)
								{
									const iCScope* scope = (NULL != var_scope)?var_scope:((NULL != mcu_decl_scope)?mcu_decl_scope:func_scope);
									parser_context->err_msg("symbol redefinition: %s aready defined in %s",
										$1->c_str(), scope->name.empty()?"this scope":scope->name.c_str());
								}
								$$ = $1;
							}
							;

/*initializer 			:	TICONST  {$$ = $1;}
							|	TDCONST {$$ = $1;}
							;*/

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
								;

%%

int ic_error(const char *s) 
{
	parser_context->err_msg(s);
	//std::cout<<"ic_error"<<std::endl;
	//exit(1);
	return 0;
}