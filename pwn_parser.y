%{
// $Id: pwn_parser.y,v 1.8 2015/04/12 21:34:14 ist175537 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#include "pwn_type.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;    /* integer value */
  double		d;
  std::string          *s;    /* symbol name or string literal */
  cdk::basic_node      *node;    /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  pwn::lvalue_node  *lvalue;
  basic_type *t;
  bool b;
};

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING
%token <expression> tNOOB
%token tIF tELSE tPRINT tREAD tREPEAT tNEXT tSTOP tRETURN tLOCAL tIMPORT

%nonassoc tIFX
%nonassoc tELSE

%left ','
%left tARG
%right '='
%left '|'
%left '&'
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY
%nonassoc '['

%type <node> stmt program decl var_decl func_decl 
%type <sequence> statements declarations params params_opt args args_opt var_decls
%type <expression> expr expr_opt
%type <lvalue> lval
%type <s> string
%type <t> type
%type <b> visibility

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

program : declarations { compiler->ast($1); }
        ;

declarations : decl              { $$ = new cdk::sequence_node(LINE, $1); }
             | declarations decl { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;

decl : var_decl ';'  { $$ = $1; }
     | func_decl { $$ = $1; }
     ;

var_decl : visibility type tIDENTIFIER          { $$ = new pwn::variable_node(LINE, $1, $2, $3, nullptr); }
         | visibility type tIDENTIFIER '=' expr { $$ = new pwn::variable_node(LINE, $1, $2, $3, $5); }
         ;

var_decls : 					{ $$ = new cdk::nil_node(LINE); }
	  | var_decls var_decl ';'		{ $$ = new cdk::sequence_node(LINE, $2, $1); }

func_decl : visibility type tIDENTIFIER '(' params_opt ')' 
              { $$ = new pwn::function_decl_node(LINE, $1, $2, $3, $5); }
          | visibility type tIDENTIFIER '(' params_opt ')' '=' expr
              { $$ = new pwn::function_def_node(LINE, $1, $2, $3, $5, $8, nullptr); }
          | visibility type tIDENTIFIER '(' params_opt ')' '{' statements '}'
              { $$ = new pwn::function_def_node(LINE, $1, $2, $3, $5, nullptr, $8); }
          | visibility type tIDENTIFIER '(' params_opt ')' '=' expr '{' statements '}'
              { $$ = new pwn::function_def_node(LINE, $1, $2, $3, $5, $8, $10); }
          ;

params : var_decl            { $$ = new cdk::sequence_node(LINE, $1); }
       | params ',' var_decl { $$ = new cdk::sequence_node(LINE, $3, $1); }
       ;

params_opt : params          { $$ = $1; }
           |                 { $$ = nullptr; }
           ;

visibility : tLOCAL  { $$ = false; }
           | tIMPORT { $$ = true; }
           |         { $$ = false; }
           ;

type : '#'          { $$ = new basic_type(4, basic_type::TYPE_INT); }
     | '%'          { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
     | '$'          { $$ = new basic_type(4, basic_type::TYPE_STRING); }
     | '*'          { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
     | '<' type '>' { $$ = new basic_type($2->size(), $2->name() | pwn_type::TYPE_CONST); } 
     | '!'          { $$ = new basic_type(0, basic_type::TYPE_VOID); }
     ;

statements :                 { $$ = new cdk::nil_node(LINE); }
           | statements stmt { $$ = new cdk::sequence_node(LINE, $2, $1); }
           ;

stmt : expr ';'                         { $$ = new pwn::evaluation_node(LINE, $1); }
     | expr '!'                         { $$ = new pwn::print_node(LINE, $1, false); }
     | expr tPRINT                      { $$ = new pwn::print_node(LINE, $1, true); }
     | tIF '(' expr ')' stmt %prec tIFX { $$ = new cdk::if_node(LINE, $3, $5); }
     | tIF '(' expr ')' stmt tELSE stmt { $$ = new cdk::if_else_node(LINE, $3, $5, $7); }
     | '{' var_decls statements '}'	{ $$ = new pwn::block_node(LINE, $2, $3); }
     | tRETURN                          { $$ = new pwn::return_node(LINE); }
     | tSTOP ';'                        { $$ = new pwn::stop_node(LINE, 1);}
     | tNEXT ';'                        { $$ = new pwn::next_node(LINE, 1);}
     | tSTOP tINTEGER ';'               { $$ = new pwn::stop_node(LINE, $2);}
     | tNEXT tINTEGER ';'               { $$ = new pwn::next_node(LINE, $2);}
     | tREPEAT '(' expr_opt ';' expr_opt ';' expr_opt ')' stmt { $$ = new pwn::repeat_node(LINE, $3, $5, $7, $9); }
     ;


expr :/*
*/
       tIDENTIFIER '(' args_opt ')' { $$ = new pwn::function_call_node(LINE, $1, $3); }
     | '(' expr ')'            { $$ = $2; }
     | '[' expr ']'            { $$ = new pwn::alloc_node(LINE, $2); } /*
*/
     | '+' expr %prec tUNARY   { $$ = new pwn::identity_node(LINE, $2); }
     | '-' expr %prec tUNARY   { $$ = new cdk::neg_node(LINE, $2); }
     | lval '?' 	       { $$ = new pwn::addressof_node(LINE, $1); } /*
*/
     | expr '*' expr           { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr           { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr           { $$ = new cdk::mod_node(LINE, $1, $3); } /*
*/
     | expr '+' expr           { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr           { $$ = new cdk::sub_node(LINE, $1, $3); } /*
*/
     | expr '<' expr           { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr           { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr           { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr           { $$ = new cdk::le_node(LINE, $1, $3); } /*
*/
     | expr tNE expr           { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr           { $$ = new cdk::eq_node(LINE, $1, $3); } /*
*/
     | '~' expr 	       { $$ = new pwn::not_node(LINE, $2); } /*
*/
     | expr '&' expr           { $$ = new pwn::and_node(LINE, $1, $3); } /*
*/
     | expr '|' expr           { $$ = new pwn::or_node(LINE, $1, $3); }/*
*/
     | lval '=' expr           { $$ = new pwn::assignment_node(LINE, $1, $3); }/*
*/
     | tINTEGER                { $$ = new cdk::integer_node(LINE, $1); }
     | tDOUBLE                 { $$ = new cdk::double_node(LINE, $1); }
     | string                  { $$ = new cdk::string_node(LINE, $1); }
     | tNOOB                   { $$ = new pwn::noob_node(LINE); }
     | '@'                     { $$ = new pwn::read_node(LINE); }
     | lval                    { $$ = new pwn::rvalue_node(LINE, $1); }
     | expr ',' expr           { $$ = new pwn::comma_node(LINE, $1, $3); }
     ;

expr_opt : expr                { $$ = $1; }
         |                     { $$ = nullptr; }
         ;

args_opt : args   { $$ = $1; }
         |        { $$ = nullptr; }
         ;

args : expr %prec tARG { $$ = new cdk::sequence_node(LINE, $1); }
     | args ',' expr   { $$ = new cdk::sequence_node(LINE, $3, $1); }
     ;

lval : tIDENTIFIER             { $$ = new pwn::identifierrr_node(LINE, *$1); }
     | expr '[' expr ']'       { $$ = new pwn::index_node(LINE, $1, $3); }
     ;

string : tSTRING               { $$ = $1;}
       | string tSTRING        { $$ = new std::string(*$1 + *$2);}
       ;

%%
