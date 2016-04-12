%{
#include "frontend/AST.hpp"

extern "C" int yylex();
extern "C" int yyparse();
void yyerror(const char *s);

extern "C" FILE *yyin;

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
%token LESS_THAN_OR_EQUAL
%token GREATER_THAN_OR_EQUAL

%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <sval> IDENTIFIER
%token <sval> STRING_LITERAL

%type <node> statements statement statementElem assignment declaration printer expr location literal block rel_expr logical_expr additive_expr multiplicative_expr unary_expr simple_expr
%type <assignOp> assignOp

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

expr: logical_expr { $$ = $1; }
    ;

logical_expr: logical_expr LOGICAL_AND rel_expr { $$ = new BinaryExpressionNode($1, BinaryOp::LOGICAL_AND, $3); }
            | logical_expr LOGICAL_OR rel_expr { $$ = new BinaryExpressionNode($1, BinaryOp::LOGICAL_OR, $3); }
            | rel_expr { $$ = $1; }
            ;

rel_expr: rel_expr '<' additive_expr { $$ = new BinaryExpressionNode($1, BinaryOp::LESS_THAN, $3); }
        | rel_expr '>' additive_expr { $$ = new BinaryExpressionNode($1, BinaryOp::GREATER_THAN, $3); }
        | rel_expr LESS_THAN_OR_EQUAL additive_expr { $$ = new BinaryExpressionNode($1, BinaryOp::LESS_THAN_OR_EQUAL, $3); }
        | rel_expr GREATER_THAN_OR_EQUAL additive_expr { $$ = new BinaryExpressionNode($1, BinaryOp::GREATER_THAN_OR_EQUAL, $3); }
        | additive_expr { $$ = $1; }
        ;

additive_expr: additive_expr '+' multiplicative_expr { $$ = new BinaryExpressionNode($1, BinaryOp::ADD, $3); }
             | additive_expr '-' multiplicative_expr { $$ = new BinaryExpressionNode($1, BinaryOp::SUBTRACT, $3); }
             | multiplicative_expr { $$ = $1; }
             ;

multiplicative_expr: multiplicative_expr '*' unary_expr { $$ = new BinaryExpressionNode($1, BinaryOp::MULTIPLY, $3); }
                   | multiplicative_expr '/' unary_expr { $$ = new BinaryExpressionNode($1, BinaryOp::DIVIDE, $3); }
                   | unary_expr { $$ = $1; }
                   ;

unary_expr: '!' unary_expr { $$ = new UnaryExpressionNode($2, '!'); }
          | simple_expr { $$ = $1; }
          ;

simple_expr: location { $$ = $1; }
           | literal { $$ = $1; }
           ;

literal: INT_LITERAL { $$ = new IntLiteralNode($1); }
       | FLOAT_LITERAL { $$ = new FloatLiteralNode($1); }
       | TRUE { $$ = new BoolLiteralNode(true); }
       | FALSE { $$ = new BoolLiteralNode(false); }
       | STRING_LITERAL { $$ = new StringLiteralNode($1); }
       ;
%%
