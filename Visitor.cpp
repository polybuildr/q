#include <map>

#include "Visitor.hpp"
#include "AST.hpp"
#include "Value.hpp"

std::map<std::string, Value*> symbols;

enum class Type { INT, CHAR, BOOL, FLOAT };

Value* Visitor::visit(StatementsListNode *node) {
    for (ASTNode *statement : (node->list)) {
        statement->accept(this);
    }
    return nullptr;
}

Value* Visitor::visit(AssignmentNode *node) {
    std::string id = dynamic_cast<LocationNode *>(node->locationNode)->id;
    Value* value = node->value->accept(this);
    symbols[id] = value;
    return nullptr;
}

Value* Visitor::visit(PrintNode *node) {
    Value *value = node->expr->accept(this);
    printf("%d\n", value->getValue());
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

    switch (node->op[0]) {
        case '+':
            if (node->op.length() == 1) {
                return new Value(value1->getValue() + value2->getValue());
            }
            break;
        case '-':
            if (node->op.length() == 1) {
                return new Value(value1->getValue() - value2->getValue());
            }
            break;
        case '*':
            if (node->op.length() == 1) {
                return new Value(value1->getValue() * value2->getValue());
            }
            break;
        case '/':
            if (node->op.length() == 1) {
                return new Value(value1->getValue() / value2->getValue());
            }
            break;
        default:
            return nullptr;
    }
    return nullptr;
}

Value* Visitor::visit(IntLiteralNode *node) {
    return new Value(node->value);
}
