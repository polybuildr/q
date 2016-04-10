%{
#include <cstdio>

#include "frontend/AST.hpp"
#include "build/q.tab.h"
#define YY_DECL extern "C" int yylex()

// TODO: int parenLevel = 0;
bool autoInsertSemicolon = true;
int lexerColumnNumber = 1;
int lexerLineNumber = 1;
%}
%%
true                    { lexerColumnNumber += yyleng; autoInsertSemicolon = true; return TRUE; }
false                   { lexerColumnNumber += yyleng; autoInsertSemicolon = true; return FALSE; }
var                     { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return VAR; }
const                   { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return CONST; }
if                      { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return IF; }
else                    { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return ELSE; }
for                     { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return FOR; }
\+=                     { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return PLUS_ASSIGN; }
-=                      { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return MINUS_ASSIGN; }
\*=                     { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return PRODUCT_ASSIGN; }
\/=                     { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return QUOTIENT_ASSIGN; }
\&\&                    { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return LOGICAL_AND; }
\|\|                    { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return LOGICAL_OR; }
\"(\\.|[^\\"])*\"       { lexerColumnNumber += yyleng; autoInsertSemicolon = true; char *s = yytext + 1; s[yyleng - 2] = '\0'; yylval.sval = strdup(s); return STRING_LITERAL; }
[+-]?[0-9]+             { lexerColumnNumber += yyleng; autoInsertSemicolon = true; yylval.ival = atoll(yytext); return INT_LITERAL; }
[+-]?[0-9]*\.[0-9]+     { lexerColumnNumber += yyleng; autoInsertSemicolon = true; yylval.fval = atof(yytext); return FLOAT_LITERAL; }
[@+\-*/={};()<>]        { lexerColumnNumber += yyleng; autoInsertSemicolon = false; return yytext[0]; }
[[:alpha:]][[:alnum:]]* { lexerColumnNumber += yyleng; autoInsertSemicolon = true; yylval.sval = strdup(yytext); return IDENTIFIER; }
[\t ]                   { lexerColumnNumber += yyleng; /* ignore */ }
\n                      { lexerColumnNumber = 1; lexerLineNumber++; if (autoInsertSemicolon) return ';'; }
%%
