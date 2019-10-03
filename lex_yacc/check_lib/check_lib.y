
/* bison input file */
%{
/******************************************************************************
    Copyright (c) 1996-2000 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SYNOPSYS Open Source License Version 1.0  (the "License"); 
  you may not use this file except in compliance with such restrictions 
  and limitations. You may obtain instructions on how to receive a copy of 
  the License at

  http://www.synopsys.com/partners/tapin/tapinprogram.html. 

  Software distributed by Original Contributor under the License is 
  distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either 
  expressed or implied. See the License for the specific language governing 
  rights and limitations under the License.

******************************************************************************/
#include <stdio.h>
int lineno;
  
%}

%token COMMA SEMI LPAR RPAR LCURLY RCURLY COLON KW_DEFINE KW_DEFINE_GROUP KW_TRUE KW_FALSE
%token NUM
%token STRING IDENT

%%

file	: {lineno = 1; } group
	;

group	: head LCURLY statements RCURLY 
        | head LCURLY RCURLY
	;

statements 	: statement 
		| statements statement 
		;

statement 	: simple_attr 
		| complex_attr 
		| define 
		| define_group 
		| group  
		;

simple_attr	: IDENT COLON attr_val SEMI
		| IDENT COLON attr_val 
		;

complex_attr 	: head  SEMI  
                | head  
		;

head	: IDENT LPAR param_list RPAR 
        | IDENT LPAR RPAR
	;

param_list  : attr_val 
            | param_list COMMA attr_val
            | param_list attr_val
	    ;

define 	: KW_DEFINE LPAR s_or_i COMMA s_or_i COMMA s_or_i RPAR SEMI  
	;

define_group : KW_DEFINE_GROUP LPAR s_or_i COMMA s_or_i RPAR SEMI
             ;

s_or_i  : STRING 
        | IDENT 
        ;

attr_val : NUM
         | s_or_i
         | KW_TRUE
         | KW_FALSE
	 ;
%%

#include <stdio.h>
int yyparse();
extern int yylex();

void yyerror(char *s)
{
  printf("line %d: %s \n", lineno, s);
}

int main()
{
  yyparse();
}
