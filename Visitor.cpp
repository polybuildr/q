#include <map>
#include <list>

#include "Visitor.hpp"
#include "AST.hpp"
#include "Value.hpp"
#include "Utils.cpp"

Visitor::Visitor() {
    std::map<std::string, std::pair<Value*, bool> > frame;
    symbols.push_back(frame);
}

Value* Visitor::visit(StatementsListNode *node) {
    std::map<std::string, std::pair<Value*, bool> > frame;
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
                if (((*it)[id]).second == false) {
                    printf("error: read-only variable '%s' is not assignable, exiting\n", id.c_str());
                    exit(1);
                }
                Value* value = node->value->accept(this);
                (*it)[id] = std::make_pair(value, true);
                return nullptr;
            }
        }
        printf("error: use of undeclared identifier '%s', exiting\n", id.c_str());
        exit(1);
    } else {
        bool isMutable = node->isMutable;
        Value* value = node->value->accept(this);
        symbols.back()[id] = std::make_pair(value, isMutable);
    }
    return nullptr;
}

Value* Visitor::visit(DeclarationNode *node) {
    std::string id = dynamic_cast<LocationNode *>(node->locationNode)->id;
    symbols.back()[id] = std::make_pair(nullptr, true); // TODO: Create `undefined` or something
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
            Value* value = (*it)[node->id].first;
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

Value* Visitor::visit(FloatLiteralNode *node) {
    return new RealNumber(node->value);
}

Value* Visitor::visit(BoolLiteralNode *node) {
    return new Boolean(node->value);
}

Value* Visitor::visit(IfNode *node) {
    Value *expr = node->condition->accept(this);
    if (Operations::getBoolValue(expr)) {
        node->thenBlock->accept(this);
    } else if (node->elseBlock) {
        node->elseBlock->accept(this);
    }
    return nullptr;
}
