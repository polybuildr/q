#include "Visitor.hpp"
#include "AST.hpp"

void Visitor::visit(StatementsListNode *node) {
    for (ASTNode *statement : (node->list)) {
        printf("statement encountered\n");
        statement->accept(this);
    }
}

void Visitor::visit(AssignmentNode *node) {
    printf("assignment\n");
}

void Visitor::visit(PrintNode *node) {
    printf("print\n");
}

void Visitor::visit(LocationNode *node) {
    printf("location\n");
}

void Visitor::visit(BinaryExpressionNode *node) {
    printf("bin expr\n");
}

void Visitor::visit(IntLiteralNode *node) {
    printf("int literal\n");
}
