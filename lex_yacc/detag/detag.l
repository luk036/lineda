%START TAG
%%

<TAG>">"             { BEGIN 0; }
<TAG>.               { /* remove */ }

"<"/[a-zA-Z!/]       { BEGIN TAG; }
"&amp;"              { printf("&"); }
"&gt;"               { printf(">"); }
"&lt;"               { printf("<"); }
.                    { ECHO; }
%%

int yywrap()
{
  return 1;
}

int main()
{
  yylex();
}
