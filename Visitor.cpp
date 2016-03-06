#include <map>

#include "Visitor.hpp"
#include "AST.hpp"
#include "Value.hpp"
#include "Utils.cpp"

std::map<std::string, Value*> symbols;

Value* Visitor::visit(StatementsListNode *node) {
    for (ASTNode *statement : (node->list)) {
        statement->accept(this);
    }
    return nullptr;
}

Value* Visitor::visit(BlockNode *node) {
    node->statementsList->accept(this);
    return nullptr;
}

Value* Visitor::visit(AssignmentNode *node) {
    std::string id = dynamic_cast<LocationNode *>(node->locationNode)->id;
    if (!node->isAlsoDeclaration) {
        if (symbols.find(id) == symbols.end()) {
            printf("error: use of undeclared identifier %s, exiting\n", id.c_str());
            exit(1);
        }
    }
    Value* value = node->value->accept(this);
    symbols[id] = value;
    return nullptr;
}

Value* Visitor::visit(DeclarationNode *node) {
    std::string id = dynamic_cast<LocationNode *>(node->locationNode)->id;
    symbols[id] = nullptr; // TODO: Create `undefined` or something
    return nullptr;
}

Value* Visitor::visit(PrintNode *node) {
    Value *value = node->expr->accept(this);
    value->print();
    return nullptr;
}

Value* Visitor::visit(LocationNode *node) {
    if (symbols.find(node->id) != symbols.end()) {
        return symbols[node->id];
    } else {
        printf("error: use of undeclared identifier %s, exiting\n", node->id.c_str());
        exit(1);
    }
    return nullptr;
}

Value* Visitor::visit(BinaryExpressionNode *node) {
    Value *value1 = node->expr1->accept(this);
    Value *value2 = node->expr2->accept(this);

    return Operations::performBinary(value1, node->op, value2);
}

Value* Visitor::visit(IntLiteralNode *node) {
    return new Integer(node->value);
}

Value* Visitor::visit(BoolLiteralNode *node) {
    return new Boolean(node->value);
}
