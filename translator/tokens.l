%{
#include <string>
#include "iCProgram.h"
#include "iCScope.h" 
#include "ParserContext.h"
#include "iCIdentifier.h"
#include "iCProcess.h"
#include "iCState.h"
#include "CCode.h"
#include "iCTimeout.h"
#include "iCHyperprocess.h"

#include "parser.hpp"

#define SAVE_TOKEN ic_lval.string = new std::string(ic_text, ic_leng)
#define TOKEN(t) (ic_lval.token = t)
//extern "C" int ic_wrap() { return 0; }
extern ParserContext* parser_context;

const int IC_TAB_SIZE = 4;

#define YY_USER_ACTION parser_context->inc_column(ic_leng);

%}

%option prefix="ic_"
%option noyywrap

%x C_CODE
%x PREPROCESSOR

%x LM_LINENUM
%x LM_FILE
%x LM_FLAGS


%x SKIP_TO_NEWLINE

%%

" "+					;
"\t"					{parser_context->to_prev_column(); parser_context->tab_column(IC_TAB_SIZE);}
"\n"					parser_context->inc_line_num();
"proctype"              return TOKEN(TPROCTYPE);
"process"				return TOKEN(TPROC			);
"state"					return TOKEN(TSTATE			);
"set"					return TOKEN(TSET			);
"start"					return TOKEN(TSTART			);
"stop"					return TOKEN(TSTOP			);
"timeout"				return TOKEN(TTIMEOUT		);
"active"				return TOKEN(TACTIVE		);
"passive"				return TOKEN(TPASSIVE		);

"reset"					return TOKEN(TRESET			);
"continue"				return TOKEN(TCONTINUE		);

"hyperprocess"			return TOKEN(THYPERPROCESS	);
"vector"				return TOKEN(TVECTOR		);	
"register"				return TOKEN(TREGISTER		);
"bit"					return TOKEN(TBIT			);
"if"					return TOKEN(TIF			);
"else"					return TOKEN(TELSE			);
"switch"				return TOKEN(TSWITCH		);
"case"					return TOKEN(TCASE			);
"default"				return TOKEN(TDEFAULT		);
"for"					return TOKEN(TFOR			);
"atomic"				return TOKEN(TATOMIC		);

"true"					return TOKEN(TTRUE			);
"false"					return TOKEN(TFALSE	 		);

"restart"				return TOKEN(TRESTART		);

"void"					SAVE_TOKEN; return TVOID	 ;
"char"					SAVE_TOKEN; return TCHAR	 ;
"int"					SAVE_TOKEN; return TINT		 ;
"short"					SAVE_TOKEN; return TSHORT	 ;
"long"					SAVE_TOKEN; return TLONG	 ;
"float"					SAVE_TOKEN; return TFLT		 ;
"double"				SAVE_TOKEN; return TDBL		 ;
"signed"				SAVE_TOKEN; return TSGND	 ;
"unsigned"				SAVE_TOKEN; return TUNSGND	 ;
"bool"					SAVE_TOKEN; return TBOOL	 ;

"const"					SAVE_TOKEN; return TCONST	 ;
"volatile"				SAVE_TOKEN; return TVOLATILE ;
"inline"				SAVE_TOKEN; return TINLINE   ;

"return"				SAVE_TOKEN; return TRETURN	 ;
"break"					SAVE_TOKEN; return TBREAK	 ;

[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return TIDENTIFIER	;
[0-9]+\.[0-9]*          SAVE_TOKEN; return TDCONST		;
[0-9]+          		{SAVE_TOKEN; return TICONST;}
"0x"[a-fA-F0-9]+		{SAVE_TOKEN; return THCONST;}
"0b"[0-1]+				{SAVE_TOKEN; return TBCONST;}
'(\\.|[^\\'])+'			{SAVE_TOKEN; return TSTRING;}
\"(\\.|[^\\"])*\"	 	{SAVE_TOKEN; return TSTRING;}

"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
"["						return TOKEN(TLBRACKET);
"]"						return TOKEN(TRBRACKET);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
":"						return TOKEN(TCOLON);
";"                     return TOKEN(TSEMIC	);
"="                     SAVE_TOKEN; return TASSGN	;
"=="                    SAVE_TOKEN; return TEQ		;
"!="                    SAVE_TOKEN; return TNEQ		;
"<"                     SAVE_TOKEN; return TLT		;
"<="                    SAVE_TOKEN; return TLE		;
">"                     SAVE_TOKEN; return TGT		;
">="                    SAVE_TOKEN; return TGE		;
"."                     SAVE_TOKEN; return TDOT		;
","                     SAVE_TOKEN; return TCOMMA	;
"+"                     SAVE_TOKEN; return TPLUS	;
"-"                     SAVE_TOKEN; return TMINUS	;
"*"                     SAVE_TOKEN; return TASTERISK		;
"/"                     SAVE_TOKEN; return TDIV		;
"%"						SAVE_TOKEN; return TPERC	;
"++"					SAVE_TOKEN; return TINC		;
"--"					SAVE_TOKEN; return TDEC		;
">>"					SAVE_TOKEN; return TRSHIFT	;
"<<"					SAVE_TOKEN; return TLSHIFT	;
"~"						SAVE_TOKEN; return TTILDE	;
"!"						SAVE_TOKEN; return TEXCLAM	;
"|"						SAVE_TOKEN; return TOR		;
"&"						SAVE_TOKEN; return TAND		;
"^"						SAVE_TOKEN; return TXOR		;
"||"					SAVE_TOKEN; return TLOR		;
"&&"					SAVE_TOKEN; return TLAND	;
">>="					SAVE_TOKEN; return TR_ASSGN	;
"<<="					SAVE_TOKEN; return TL_ASSGN	;
"+="					SAVE_TOKEN; return TPLUS_ASSGN	;
"-="					SAVE_TOKEN; return TMINUS_ASSGN	;
"*="					SAVE_TOKEN; return TASTERISK_ASSGN	;
"/="					SAVE_TOKEN; return TDIV_ASSGN	;
"%="					SAVE_TOKEN; return TPERC_ASSGN	;
"&="					SAVE_TOKEN; return TAND_ASSGN	;
"^="					SAVE_TOKEN; return TXOR_ASSGN	;
"|="					SAVE_TOKEN; return TOR_ASSGN	;

(?# ----------------------------------------------------------------------------------------------- )
(?# Preprocessor line markers parser state machine													)
(?# ----------------------------------------------------------------------------------------------- )
^(([ \t]*)\#("line"?))				BEGIN(LM_LINENUM);
<LM_LINENUM>[0-9]+					{
										parser_context->set_line(atoi(ic_text)); 
										parser_context->reset_column(); 
										BEGIN(LM_FILE);
									}
<LM_FILE>\"[^"\\\n]*\"				{
										std::string* str = new std::string(ic_text, ic_leng);
										str->erase(0, 1); // trim the starting quote
										str->erase(str->end()-1, str->end()); // trim the ending quote
										parser_context->set_file(*str);
										delete str; 
										BEGIN(LM_FLAGS);
									}
<LM_FLAGS>[0-9]+					;
<LM_FILE,LM_FLAGS>\n				BEGIN(INITIAL);
<LM_LINENUM,LM_FILE,LM_FLAGS>[ \t]*	;

\#							parser_context->err_msg("stray # in program"); BEGIN(SKIP_TO_NEWLINE);
<SKIP_TO_NEWLINE>[^\n]		;
<SKIP_TO_NEWLINE>\n			BEGIN(INITIAL);

(?# ----------------------------------------------------------------------------------------------- )
(?# Multi-line C code 																				)
(?# ----------------------------------------------------------------------------------------------- )
\${2}						BEGIN(C_CODE); ic_lval.string = new std::string;
<C_CODE>\${2}				BEGIN(INITIAL); return TCCODEEXPR;
<C_CODE>((\$?)[^\$\n]+)*	/*SAVE_TOKEN; return TCCODELINE;*/(*ic_lval.string)+=" "+std::string(ic_text);
<C_CODE>\n					parser_context->inc_line_num();	

(?# ----------------------------------------------------------------------------------------------- )
(?# Inline C code 																					)
(?# ----------------------------------------------------------------------------------------------- )
(\$([^\$\n]+)([^\n]*))/\n	{
								SAVE_TOKEN;
								std::string* str = ic_lval.string;
								//parser_context->err_msg("adding c code line %s", str->c_str());
								str->erase(0, 1); // trim the dollar sign
								//str->erase(str->end()-1, str->end()); // trim the newline
								//parser_context->inc_line_num();
								return TCCODELINE;
							}
\$/\n						/*parser_context->inc_line_num();*/




.                       	printf("Unknown token at line %ul\n", parser_context->line()); yyterminate();

%%

