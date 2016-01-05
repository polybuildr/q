%{
#include <cstdio>

#include "AST.hpp"
#include "Visitor.hpp"

extern "C" int yylex();
extern "C" int yyparse();
void yyerror(const char *s);

extern "C" FILE *yyin;
using namespace std;

StatementsListNode *program;
%}

%union {
    int ival;
    char *sval;
    ASTNode *node;
}

%token ASSIGN_OP
%token TRUE
%token FALSE
%token LOGICAL_AND
%token LOGICAL_OR

%token <ival> INT_LITERAL
%token <sval> IDENTIFIER

%type <node> statements statement assignment printer expr location literal

%left '-' '+'
%left '*' '/'

%%
statements: /* empty */ { $$ = new StatementsListNode(); program = dynamic_cast<StatementsListNode *>$$; }
          | statements statement { $$ = $1; dynamic_cast<StatementsListNode *>($$)->append($2); };

statement: assignment
         | printer
         ;

assignment: location ASSIGN_OP expr { $$ = new AssignmentNode($1, $3); }
          ;

location: IDENTIFIER { $$ = new LocationNode($1); }
        ;

printer: '@' expr { $$ = new PrintNode($2); }
       ;

expr: literal { $$ = $1; }
    | location
    | expr '+' expr { $$ = new BinaryExpressionNode($1, "+", $3); }
    | expr '-' expr { $$ = new BinaryExpressionNode($1, "-", $3); }
    | expr '*' expr { $$ = new BinaryExpressionNode($1, "*", $3); }
    | expr '/' expr { $$ = new BinaryExpressionNode($1, "/", $3); }
    | expr LOGICAL_AND expr { $$ = new BinaryExpressionNode($1, "&&", $3); }
    | expr LOGICAL_OR expr { $$ = new BinaryExpressionNode($1, "||", $3); }
    ;

literal: INT_LITERAL { $$ = new IntLiteralNode($1); }
       | TRUE { $$ = new BoolLiteralNode(true); }
       | FALSE { $$ = new BoolLiteralNode(false); }
       ;
%%
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("usage: q FILE\n");
        exit(1);
    }
    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        printf("error: could not open \"%s\"\n", argv[1]);
        exit(1);
    }
    yyin = inputFile;
    do {
        yyparse();
    } while (!feof(yyin));

    Visitor *v = new Visitor();
    program->accept(v);

    return 0;
}

void yyerror(const char *s) {
    printf("ERROR! Exiting.\n");
    exit(1);
}
