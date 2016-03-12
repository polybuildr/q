#include <map>
#include <list>
#include <memory>

#include "Visitor.hpp"
#include "AST.hpp"
#include "Value.hpp"
#include "Utils.cpp"

void Visitor::pushNewSymbolFrame() {
    std::map<std::string, std::pair<std::shared_ptr<Value>, bool> > frame;
    symbols.push_back(frame);
}

void Visitor::popSymbolFrame() {
    symbols.pop_back();
}

Visitor::Visitor() {
    pushNewSymbolFrame();
}

std::shared_ptr<Value> Visitor::visit(StatementsListNode *node) {
    pushNewSymbolFrame();
    for (ASTNode *statement : (node->list)) {
        statement->accept(this);
    }
    popSymbolFrame();
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(BlockNode *node) {
    node->statementsList->accept(this);
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(AssignmentNode *node) {
    std::string id = dynamic_cast<LocationNode *>(node->locationNode)->id;
    if (!node->isAlsoDeclaration) {
        for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
            if ((*it).find(id) != (*it).end()) {
                if (((*it)[id]).second == false) {
                    printf("error: read-only variable '%s' is not assignable, exiting\n", id.c_str());
                    exit(1);
                }
                std::shared_ptr<Value> value(node->value->accept(this));
                (*it)[id] = std::make_pair(value, true);
                return nullptr;
            }
        }
        printf("error: use of undeclared identifier '%s', exiting\n", id.c_str());
        exit(1);
    } else {
        bool isMutable = node->isMutable;
        std::shared_ptr<Value> value(node->value->accept(this));
        symbols.back()[id] = std::make_pair(value, isMutable);
    }
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(DeclarationNode *node) {
    std::string id = dynamic_cast<LocationNode *>(node->locationNode)->id;
    symbols.back()[id] = std::make_pair(nullptr, true); // TODO: Create `undefined` or something
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(PrintNode *node) {
    std::shared_ptr<Value> value(node->expr->accept(this));
    value->print();
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(LocationNode *node) {
    for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
        if ((*it).find(node->id) != (*it).end()) {
            std::shared_ptr<Value> value((*it)[node->id].first);
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

std::shared_ptr<Value> Visitor::visit(BinaryExpressionNode *node) {
    std::shared_ptr<Value> value1(node->expr1->accept(this));
    std::shared_ptr<Value> value2(node->expr2->accept(this));

    return std::make_shared<Value>(Operations::performBinary(value1, node->op, value2));
}

std::shared_ptr<Value> Visitor::visit(IntLiteralNode *node) {
    return std::make_shared<Value>(node->value);
}

std::shared_ptr<Value> Visitor::visit(FloatLiteralNode *node) {
    return std::make_shared<Value>(node->value);
}

std::shared_ptr<Value> Visitor::visit(BoolLiteralNode *node) {
    return std::make_shared<Value>(node->value);
}

std::shared_ptr<Value> Visitor::visit(IfNode *node) {
    std::shared_ptr<Value> expr(node->condition->accept(this));
    if (getBoolValue(expr)) {
        node->thenBlock->accept(this);
    } else if (node->elseBlock) {
        node->elseBlock->accept(this);
    }
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(ForLoopNode *node) {
    pushNewSymbolFrame();
    node->init->accept(this);
    std::shared_ptr<Value> condition(node->condition->accept(this));
    while (getBoolValue(condition)) {
        node->body->accept(this);
        node->increment->accept(this);
        condition = node->condition->accept(this);
    }
    popSymbolFrame();
    return nullptr;
}
