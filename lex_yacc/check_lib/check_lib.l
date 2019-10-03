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
#include "check_lib_yacc.h"

extern int lineno;
char *search_string_for_linefeeds(char *str);
%}

%x comment
%x stringx
%x include

VOLTAGE VDD|VCC|VSS
EXPR_OP "+"|"-"|"*"|"/"
FLOAT [-+]?([0-9]+\.?[0-9]*([Ee][-+]?[0-9]+)?|[0-9]*\.[0-9]*([Ee][-+]?[0-9]+)?)
SP  [ \t]*
%%

\,	{return COMMA;}
\;	{return SEMI;}
\(	{return LPAR;}
\)	{return RPAR;}
\{	{return LCURLY;}
\}[ \t]*\;?	{return RCURLY;}
[ \t]?\:	{return COLON;}
include_file[ \t]*\(	BEGIN(include);

[-+]?([0-9]+\.?[0-9]*([Ee][-+]?[0-9]+)?|[0-9]*\.[0-9]*([Ee][-+]?[0-9]+)?) {return NUM;}

{VOLTAGE}{SP}{EXPR_OP}{SP}{FLOAT}|{FLOAT}{SP}{EXPR_OP}{SP}{VOLTAGE} { return STRING;}

[A-Za-z!@#$%^&_+=\|~\?][A-Za-z0-9!@#$%^&_+=\|~\?]*[\<\{\[\(][-0-9:]+[\]\}\>\)] { return IDENT; }

[-+]?[0-9]*\.?[0-9]+([Ee][-+]?[0-9]+)?[ \t]*[-\+\*\/][ 	]*[-+]?[0-9]*\.?[0-9]+([Ee][-+]?[0-9]+)?         {return STRING;}
"define"                 {return KW_DEFINE;}
"define_group"           {return KW_DEFINE_GROUP;}
[Tt][Rr][Uu][Ee]         {return KW_TRUE;}
[Ff][Aa][Ll][Ss][Ee]     {return KW_FALSE;}
\\?\n	                 {lineno++;}
\\[ \t]+\n	{printf("line %d -- Continuation char followed by spaces or tabs!\n\n", lineno); lineno++; }
\r  
\t  
" " 

"/*"	BEGIN(comment);
\"	BEGIN(stringx);

<comment>[^*\n]*        /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>\n             ++lineno;
<comment>"*"+"/"	BEGIN(INITIAL);

<stringx>\"	{ BEGIN(INITIAL);  return STRING; }
<stringx>\n     { BEGIN(INITIAL);  return STRING; }
<stringx>\\\n 	{ lineno++; }
<stringx>\\.
<stringx>[^\\\n\"]+

<include>[ \t]*         
<include>[^ \t\n);]+	
<include>")"
<include>";" 

[a-zA-Z0-9!@#$%^&_+=\|~\?<>\.\-]+ { return IDENT;}

%%

int yywrap()
{
    return(1);
}

char *search_string_for_linefeeds(char *str)
{
   char *s;
   s = str;
   while (*s){ if( *s++ == '\n' ) {lineno++; if( *(s-2) != '\\' ){printf("Warning: line %d: String constant spanning input lines does not use continuation character.\n",lineno);} } }
   return str;
}

