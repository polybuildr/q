%{
#include <cstdio>

#include "AST.hpp"
#include "q.tab.h"
#define YY_DECL extern "C" int yylex()

//TODO: int parenLevel = 0;
bool autoInsertSemicolon = true;
using namespace std;
%}
%%
true { autoInsertSemicolon = true; return TRUE; }
false { autoInsertSemicolon = true; return FALSE; }
var { autoInsertSemicolon = false; return VAR; }
\&\& { autoInsertSemicolon = false; return LOGICAL_AND; }
\|\| { autoInsertSemicolon = false; return LOGICAL_OR; }
[[:digit:]]+ { autoInsertSemicolon = true; yylval.ival = atoi(yytext); return INT_LITERAL; }
[@+\-*/={};] { autoInsertSemicolon = false; return yytext[0]; }
[[:alpha:]][[:alnum:]]* { autoInsertSemicolon = true; yylval.sval = strdup(yytext); return IDENTIFIER; }
[\t ] { /* ignore */ }
\n { if (autoInsertSemicolon) return ';'; }
%%
