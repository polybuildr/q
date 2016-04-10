#include <cstdio>
#include <cstdint>

#include "AST.hpp"
#include "Visitor.hpp"

extern "C" int yylex();
extern "C" int yyparse();
void yyerror(const char *s);

extern "C" FILE *yyin;
extern "C" int lexerLineNumber;
extern "C" int lexerColumnNumber;

extern StatementsListNode *program;

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
    fprintf(stderr, "error: could not parse program\nlocation guess: line %d, column %d\n", lexerLineNumber, lexerColumnNumber);
    exit(1);
}
