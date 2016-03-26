#include <map>
#include <list>
#include <iostream>

#include "Globals.hpp"
#include "Visitor.hpp"
#include "AST.hpp"
#include "Value.hpp"
#include "Utils.cpp"


void Visitor::pushNewSymbolFrame() {
    std::map<std::string, Value> frame;
    symbols.push_back(frame);
}

void Visitor::popSymbolFrame() {
    symbols.pop_back();
}

Visitor::Visitor() {
    pushNewSymbolFrame();
}

void Visitor::callAndGetValueFrom(ASTNode *node, Value &result) {
    switch (node->nodeType) {
        case NodeType::LOCATION:
            visit(static_cast<LocationNode *>(node), result);
            break;
        case NodeType::BINARY_EXPR:
            visit(static_cast<BinaryExpressionNode *>(node), result);
            break;
        case NodeType::INT_LITERAL:
            visit(static_cast<IntLiteralNode *>(node), result);
            break;
        case NodeType::BOOL_LITERAL:
            visit(static_cast<BoolLiteralNode *>(node), result);
            break;
        case NodeType::FLOAT_LITERAL:
            visit(static_cast<FloatLiteralNode *>(node), result);
            break;
        case NodeType::STRING_LITERAL:
            visit(static_cast<StringLiteralNode *>(node), result);
            break;
        default:
            printf("error: unknown node type, exiting\n");
            exit(1);
    }
}

void Visitor::callVoidVisitOn(ASTNode *node) {
    switch (node->nodeType) {
        case NodeType::ASSIGNMENT:
            visit(static_cast<AssignmentNode *>(node));
            break;
        case NodeType::DECLARATION:
            visit(static_cast<DeclarationNode *>(node));
            break;
        case NodeType::STATEMENTS_LIST:
            visit(static_cast<StatementsListNode *>(node));
            break;
        case NodeType::BLOCK:
            visit(static_cast<BlockNode *>(node));
            break;
        case NodeType::IF:
            visit(static_cast<IfNode *>(node));
            break;
        case NodeType::FOR:
            visit(static_cast<ForLoopNode *>(node));
            break;
        case NodeType::PRINT:
            visit(static_cast<PrintNode *>(node));
            break;
        default:
            printf("error: unknown node type, exiting\n");
            exit(1);
    }
}

void Visitor::visit(StatementsListNode *node) {
    pushNewSymbolFrame();
    for (ASTNode *statement: node->list) {
        callVoidVisitOn(statement);
    }
    popSymbolFrame();
}

void Visitor::visit(BlockNode *node) {
    visit(static_cast<StatementsListNode *>(node->statementsList));
}

void Visitor::visit(AssignmentNode *node) {
    std::string id = static_cast<LocationNode *>(node->locationNode)->id;
    if (!node->isAlsoDeclaration) {
        for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
            auto v = (*it).find(id);
            if (v != (*it).end()) {
                if (v->second.constant == true) {
                    printf("error: read-only variable '%s' is not assignable, exiting\n", id.c_str());
                    exit(1);
                }
                Value value;
                callAndGetValueFrom(node->value, value);
                v->second = value;
                return;
            }
        }
        printf("error: use of undeclared identifier '%s', exiting\n", id.c_str());
        exit(1);
    } else {
        Value value;
        callAndGetValueFrom(node->value, value);
        value.constant = !node->isMutable;
        symbols.back()[id] = value;
    }
}

void Visitor::visit(DeclarationNode *node) {
    std::string id = static_cast<LocationNode *>(node->locationNode)->id;
    symbols.back()[id] = Value();
}

void Visitor::visit(PrintNode *node) {
    Value value;
    callAndGetValueFrom(node->expr, value);
    value.print();
}

void Visitor::visit(LocationNode *node, Value &result) {
    for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
        if ((*it).find(node->id) != (*it).end()) {
            result = (*it)[node->id];
            if (result.type == ValueType::UNDEFINED) {
                printf("error: use of unitialised identifier '%s', exiting\n", node->id.c_str());
                exit(1);
            }
            return;
        }
    }
    printf("error: use of undeclared identifier '%s', exiting\n", node->id.c_str());
    exit(1);
}

void Visitor::visit(BinaryExpressionNode *node, Value &result) {
    Value value1, value2;
    callAndGetValueFrom(node->expr1, value1);
    callAndGetValueFrom(node->expr2, value2);

    Operations::performBinary(result, value1, node->op, value2);
}

void Visitor::visit(IntLiteralNode *node, Value &result) {
    result.set(node->value);
}

void Visitor::visit(FloatLiteralNode *node, Value &result) {
    result.set(node->value);
}

void Visitor::visit(BoolLiteralNode *node, Value &result) {
    result.set(node->value);
}

void Visitor::visit(StringLiteralNode *node, Value &result) {
    String *s = new String(node->value);
    int idx;
    if (pool.freeStringsList.empty()) {
        pool.strings.push_back(std::make_pair(1, s));
        idx = static_cast<int>(pool.strings.size()) - 1;
    }
    else {
        idx = *(pool.freeStringsList.begin());
        pool.freeStringsList.pop_front();
        pool.strings[idx] = std::make_pair(1, s);
    }
    result.set(idx, ValueType::STRING);
}

void Visitor::visit(IfNode *node) {
    Value condition;
    callAndGetValueFrom(node->condition, condition);
    if (getBoolValue(condition)) {
        callVoidVisitOn(node->thenBlock);
    } else if (node->elseBlock) {
        callVoidVisitOn(node->elseBlock);
    }
}

void Visitor::visit(ForLoopNode *node) {
    pushNewSymbolFrame();
    callVoidVisitOn(node->init);
    Value condition;
    callAndGetValueFrom(node->condition, condition);
    while (getBoolValue(condition)) {
        callVoidVisitOn(node->body);
        callVoidVisitOn(node->increment);
        callAndGetValueFrom(node->condition, condition);
    }
    popSymbolFrame();
}
