#include <map>
#include <list>

#include "Visitor.hpp"
#include "AST.hpp"
#include "Value.hpp"
#include "Utils.cpp"

Visitor::Visitor() {
    std::map<std::string, Value*> frame;
    symbols.push_back(frame);
}

Value* Visitor::visit(StatementsListNode *node) {
    std::map<std::string, Value*> frame;
    symbols.push_back(frame);
    for (ASTNode *statement : (node->list)) {
        statement->accept(this);
    }
    symbols.pop_back();
    return nullptr;
}

Value* Visitor::visit(BlockNode *node) {
    node->statementsList->accept(this);
    return nullptr;
}

Value* Visitor::visit(AssignmentNode *node) {
    std::string id = dynamic_cast<LocationNode *>(node->locationNode)->id;
    if (!node->isAlsoDeclaration) {
        for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
            if ((*it).find(id) != (*it).end()) {
                Value* value = node->value->accept(this);
                (*it)[id] = value;
                return nullptr;
            }
        }
        printf("error: use of undeclared identifier '%s', exiting\n", id.c_str());
        exit(1);
    } else {
        Value* value = node->value->accept(this);
        symbols.back()[id] = value;
    }
    return nullptr;
}

Value* Visitor::visit(DeclarationNode *node) {
    std::string id = dynamic_cast<LocationNode *>(node->locationNode)->id;
    symbols.back()[id] = nullptr; // TODO: Create `undefined` or something
    return nullptr;
}

Value* Visitor::visit(PrintNode *node) {
    Value *value = node->expr->accept(this);
    value->print();
    return nullptr;
}

Value* Visitor::visit(LocationNode *node) {
    for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
        if ((*it).find(node->id) != (*it).end()) {
            Value* value = (*it)[node->id];
            if (value == nullptr) {
                printf("error: use of unitialised identifier '%s', exiting\n", node->id.c_str());
                exit(1);
            }
            return value;
        }
    }
    printf("error: use of undeclared identifier '%s', exiting\n", node->id.c_str());
    exit(1);
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
