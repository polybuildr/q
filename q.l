%{
#include <cstdio>

#include "AST.hpp"
#include "q.tab.h"
#define YY_DECL extern "C" int yylex()

// TODO: int parenLevel = 0;
bool autoInsertSemicolon = true;
%}
%%
true                    { autoInsertSemicolon = true; return TRUE; }
false                   { autoInsertSemicolon = true; return FALSE; }
var                     { autoInsertSemicolon = false; return VAR; }
const                   { autoInsertSemicolon = false; return CONST; }
if                      { autoInsertSemicolon = false; return IF; }
else                    { autoInsertSemicolon = false; return ELSE; }
for                     { autoInsertSemicolon = false; return FOR; }
\+=                     { autoInsertSemicolon = false; return PLUS_ASSIGN; }
-=                      { autoInsertSemicolon = false; return MINUS_ASSIGN; }
\*=                     { autoInsertSemicolon = false; return PRODUCT_ASSIGN; }
\/=                     { autoInsertSemicolon = false; return QUOTIENT_ASSIGN; }
\&\&                    { autoInsertSemicolon = false; return LOGICAL_AND; }
\|\|                    { autoInsertSemicolon = false; return LOGICAL_OR; }
\"(\\.|[^\\"])*\"       { autoInsertSemicolon = true; char *s = yytext + 1; s[yyleng - 2] = '\0'; yylval.sval = strdup(s); return STRING_LITERAL; }
[+-]?[0-9]+             { autoInsertSemicolon = true; yylval.ival = atoll(yytext); return INT_LITERAL; }
[+-]?[0-9]*\.[0-9]+     { autoInsertSemicolon = true; yylval.fval = atof(yytext); return FLOAT_LITERAL; }
[@+\-*/={};()<>]        { autoInsertSemicolon = false; return yytext[0]; }
[[:alpha:]][[:alnum:]]* { autoInsertSemicolon = true; yylval.sval = strdup(yytext); return IDENTIFIER; }
[\t ]                   { /* ignore */ }
\n                      { if (autoInsertSemicolon) return ';'; }
%%
