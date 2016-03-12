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
    float fval;
    char *sval;
    ASTNode *node;
}

%token TRUE
%token FALSE
%token LOGICAL_AND
%token LOGICAL_OR
%token VAR
%token CONST
%token IF
%token FOR

%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <sval> IDENTIFIER

%type <node> statements statement statementElem assignment declaration printer expr location literal block

%left '<' '>'
%left '-' '+'
%left '*' '/'
%left LOGICAL_OR LOGICAL_AND

%right "then" ELSE

%%
PROGRAM: statements { program = dynamic_cast<StatementsListNode *>$1; }

statements: /* empty */ { $$ = new StatementsListNode();  }
          | statements statement { $$ = $1; dynamic_cast<StatementsListNode *>($$)->append($2); }
          | statements ';' { $$ = $1; }
          ;

statement: statementElem ';' 
         | block 
         | IF '(' expr ')' statement { $$ = new IfNode($3, $5); } %prec "then"
         | IF '(' expr ')' statement ELSE statement { $$ = new IfNode($3, $5, $7); }
         | FOR '(' statementElem ';' expr ';' statementElem ')' statement { $$ = new ForLoopNode($3, $5, $7, $9); }
         ;

statementElem: assignment 
       | declaration
       | printer 
       ;

block: '{' statements '}' { $$ = new BlockNode($2); }
     ;

declaration: VAR location { $$ = new DeclarationNode($2); }
           ;

assignment: VAR location '=' expr { $$ = new AssignmentNode($2, $4, true); }
          | location '=' expr { $$ = new AssignmentNode($1, $3); }
          | CONST location '=' expr { $$ = new AssignmentNode($2, $4, true, false); }
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
    | expr '<' expr { $$ = new BinaryExpressionNode($1, "<", $3); }
    | expr '>' expr { $$ = new BinaryExpressionNode($1, ">", $3); }
    | expr LOGICAL_AND expr { $$ = new BinaryExpressionNode($1, "&&", $3); }
    | expr LOGICAL_OR expr { $$ = new BinaryExpressionNode($1, "||", $3); }
    ;

literal: INT_LITERAL { $$ = new IntLiteralNode($1); }
       | FLOAT_LITERAL { $$ = new FloatLiteralNode($1); }
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