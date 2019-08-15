%{

#include <string>
#include <iostream>
#include "prep_parser.hpp"


#define SAVE_TOKEN ic_prep_lval.string = new std::string(ic_prep_text, ic_prep_leng)
#define TOKEN(t) (ic_prep_lval.token = t)

%}


%option prefix="ic_prep_"
%option noyywrap
%x COMMENT

%%
"#include"([ \t]*)\"([\.\/a-zA-Z0-9_]*)(\")([ \t]*)"\n"	{
											SAVE_TOKEN;
											std::string* str = ic_prep_lval.string;
											str->erase(str->end()-1, str->end()); // trim the newline
											//std::cout<<"include is \""<<*str<<"\""<<std::endl;//debug
										}
"/*"					BEGIN(COMMENT);
<COMMENT>"*/"			BEGIN(INITIAL);
<COMMENT>[^*\n]+		{
							//std::cout<<"Found section comment"<<std::endl;
							SAVE_TOKEN;
							std::string* str = ic_prep_lval.string;
							std::cout<<"comment line \""<<*str<<"\""<<std::endl;
							//return TCOMMENTLINE;
						}
<COMMENT>"*"			;
<COMMENT>\n				;//parser_context->inc_line_num();           		
"//".*"\n"				{
							//std::cout<<"Found inline comment"<<std::endl;
							SAVE_TOKEN;
							std::string* str = ic_prep_lval.string;
							str->erase(0, 2); // trim the slashes
							str->erase(str->end()-1, str->end()); // trim the newline
							std::cout<<"comment line \""<<*str<<"\""<<std::endl;
							//return TCOMMENTLINE;
						}
"\n"					;//parser_context->inc_line_num(); 
.                       ;//printf("Unknown token\n" ); //yyterminate();
%%

