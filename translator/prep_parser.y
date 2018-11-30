
%{
	#include <iostream>
	#include <string>

    extern int ic_prep_lex();
	int ic_prep_error(const char *s);
%}

/***********************************************/
/*                DATA TYPES                   */
/***********************************************/
%union 
{
	std::string *string;
	int token;
	
}

%token <token> TWORD			"word"

/***********************************************/
/*                  DESTRUCTORS                */
/***********************************************/


/*************************************************************************************************/
/*                                        GRAMMAR                                                */
/*************************************************************************************************/
%start program /*Start symbol*/
%error-verbose /*Extended error reporting*/
%define api.prefix {ic_prep_}

%%

program		:	TWORD
				{
					//std::cout<<"program is \""<<$1<<"\""<<std::endl;
					$1;
				}

%%

int ic_prep_error(const char *s) 
{
	//parser_context->err_msg(s);
	return 0;
}

