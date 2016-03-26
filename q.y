%{
#include <cstdio>
#include <cstdint>

#include "AST.hpp"
#include "Visitor.hpp"
#include "Globals.cpp"

extern "C" int yylex();
extern "C" int yyparse();
void yyerror(const char *s);

extern "C" FILE *yyin;
using namespace std;

StatementsListNode *program;
%}

%union {
    int64_t ival;
    double fval;
    char *sval;
    ASTNode *node;
    AssignOp assignOp;
}

%token TRUE
%token FALSE
%token LOGICAL_AND
%token LOGICAL_OR
%token VAR
%token CONST
%token IF
%token FOR
%token PLUS_ASSIGN
%token MINUS_ASSIGN
%token PRODUCT_ASSIGN
%token QUOTIENT_ASSIGN

%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <sval> IDENTIFIER
%token <sval> STRING_LITERAL

%type <node> statements statement statementElem assignment declaration printer expr location literal block
%type <assignOp> assignOp

%left LOGICAL_OR
%left LOGICAL_AND
%left '<' '>'
%left '-' '+'
%left '*' '/'

%right "then" ELSE

%%
PROGRAM: statements { program = static_cast<StatementsListNode *>$1; }

statements: /* empty */ { $$ = new StatementsListNode();  }
          | statements statement { $$ = $1; static_cast<StatementsListNode *>($$)->append($2); }
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

assignment: VAR location assignOp expr { $$ = new AssignmentNode($2, $3, $4, true); }
          | location assignOp expr { $$ = new AssignmentNode($1, $2, $3); }
          | CONST location assignOp expr { $$ = new AssignmentNode($2, $3, $4, true, false); }
          ;

assignOp: '=' { $$ = AssignOp::SIMPLE; }
        | PLUS_ASSIGN { $$ = AssignOp::COMPOUND_SUM; }
        | MINUS_ASSIGN { $$ = AssignOp::COMPOUND_DIFFERENCE; }
        | PRODUCT_ASSIGN { $$ = AssignOp::COMPOUND_PRODUCT; }
        | QUOTIENT_ASSIGN { $$ = AssignOp::COMPOUND_QUOTIENT; }
        ;

location: IDENTIFIER { $$ = new LocationNode($1); free($1); }
        ;

printer: '@' expr { $$ = new PrintNode($2); }
       ;

expr: literal { $$ = $1; }
    | location
    | expr '+' expr { $$ = new BinaryExpressionNode($1, BinaryOp::ADD, $3); }
    | expr '-' expr { $$ = new BinaryExpressionNode($1, BinaryOp::SUBTRACT, $3); }
    | expr '*' expr { $$ = new BinaryExpressionNode($1, BinaryOp::MULTIPLY, $3); }
    | expr '/' expr { $$ = new BinaryExpressionNode($1, BinaryOp::DIVIDE, $3); }
    | expr '<' expr { $$ = new BinaryExpressionNode($1, BinaryOp::LESS_THAN, $3); }
    | expr '>' expr { $$ = new BinaryExpressionNode($1, BinaryOp::GREATER_THAN, $3); }
    | expr LOGICAL_AND expr { $$ = new BinaryExpressionNode($1, BinaryOp::LOGICAL_AND, $3); }
    | expr LOGICAL_OR expr { $$ = new BinaryExpressionNode($1, BinaryOp::LOGICAL_OR, $3); }
    ;

literal: INT_LITERAL { $$ = new IntLiteralNode($1); }
       | FLOAT_LITERAL { $$ = new FloatLiteralNode($1); }
       | TRUE { $$ = new BoolLiteralNode(true); }
       | FALSE { $$ = new BoolLiteralNode(false); }
       | STRING_LITERAL { $$ = new StringLiteralNode($1); }
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
    v->callVoidVisitOn(program);

    return 0;
}

void yyerror(const char *s) {
    printf("ERROR! Exiting.\n");
    exit(1);
}
