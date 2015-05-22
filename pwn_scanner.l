%option c++ yyclass="pwn_scanner" outfile="pwn_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: pwn_scanner.l,v 1.5 2015/05/18 08:46:24 ist176133 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "scope.h"
#include "pwn_scanner.h"
#include "pwn_parser.tab.h"

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT

WHITESPACE              [ \t\n\r]+
H                       [0-9a-fA-F]
INT			0|([1-9][0-9]*)
D			[0-9]
%%

"//".*                 ; /* ignore comments */
"/*"			yy_push_state(X_COMMENT);

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"<>"                   return tNE;
"!!"			return tPRINT;

"if"                   return tIF; 
"else"                 return tELSE; 
"import"		return tIMPORT;
"local"			return tLOCAL;
"next"			return tNEXT;
"stop"			return tSTOP;
"repeat"		return tREPEAT;
"noob"			return tNOOB;
"return"		return tRETURN;

[_A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"           yy_pop_state(); return tSTRING;
<X_STRING>\\{H}{H}?	*yylval.s += static_cast<char>(strtol(yytext+1, nullptr, 16)); 
<X_STRING>\\\"         *yylval.s += "\"";
<X_STRING>\\\\		*yylval.s += "\\";
<X_STRING>\\n		*yylval.s += "\n";
<X_STRING>\\r		*yylval.s += "\r";
<X_STRING>\\t		*yylval.s += "\t";
<X_STRING>.            *yylval.s += yytext;
<X_STRING>\n           yyerror("newline in string");

<X_COMMENT>"/*"		yy_push_state(X_COMMENT);
<X_COMMENT>"*/"		yy_pop_state();
<X_COMMENT>.|\n		;

{INT}			yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;
0[xX]{H}+		yylval.i = strtol(yytext, nullptr, 16); return tINTEGER;

({D}+\.{D}*|{D}*\.{D}+)([eE][-+]?{D}+)?	yylval.d = strtod(yytext, nullptr); return tDOUBLE; /* for .1e2 or 3.e4 or 5.06e7 */
[-+]?{D}+[eE][-+]?{D} 				yylval.d = strtod(yytext, nullptr); return tDOUBLE; /* for 1e2, for example*/

[-()<>=+*/%;{}!@,~&|?\[\]$#]     return *yytext;

{WHITESPACE}		; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int pwn_scanner::yywrap() { return 1; }
