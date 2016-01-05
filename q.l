%{
#include <cstdio>

#include "AST.hpp"
#include "q.tab.h"
#define YY_DECL extern "C" int yylex()

using namespace std;
%}
%%
true { return TRUE; }
false { return FALSE; }
[[:digit:]]+ { yylval.ival = atoi(yytext); return INT_LITERAL; }
[@+\-*/] { return yytext[0]; }
[[:alpha:]][[:alnum:]]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
\:\= { return ASSIGN_OP; }
[\t \n] { /* ignore */ }
%%
