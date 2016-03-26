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

Value Visitor::callAndGetValueFrom(ASTNode *node) {
    switch (node->nodeType) {
        case NodeType::LOCATION:
            return visit(static_cast<LocationNode *>(node));
            break;
        case NodeType::BINARY_EXPR:
            return visit(static_cast<BinaryExpressionNode *>(node));
            break;
        case NodeType::INT_LITERAL:
            return visit(static_cast<IntLiteralNode *>(node));
            break;
        case NodeType::BOOL_LITERAL:
            return visit(static_cast<BoolLiteralNode *>(node));
            break;
        case NodeType::FLOAT_LITERAL:
            return visit(static_cast<FloatLiteralNode *>(node));
            break;
        case NodeType::STRING_LITERAL:
            return visit(static_cast<StringLiteralNode *>(node));
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
                v->second = callAndGetValueFrom(node->value);
                return;
            }
        }
        printf("error: use of undeclared identifier '%s', exiting\n", id.c_str());
        exit(1);
    } else {
        Value value = callAndGetValueFrom(node->value);
        value.constant = !node->isMutable;
        symbols.back()[id] = value;
    }
}

void Visitor::visit(DeclarationNode *node) {
    std::string id = static_cast<LocationNode *>(node->locationNode)->id;
    symbols.back()[id] = Value();
}

void Visitor::visit(PrintNode *node) {
    Value value = callAndGetValueFrom(node->expr);
    value.print();
}

Value Visitor::visit(LocationNode *node) {
    for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
        if ((*it).find(node->id) != (*it).end()) {
            Value result = (*it)[node->id];
            if (result.type == ValueType::UNDEFINED) {
                printf("error: use of unitialised identifier '%s', exiting\n", node->id.c_str());
                exit(1);
            }
            return result;
        }
    }
    printf("error: use of undeclared identifier '%s', exiting\n", node->id.c_str());
    exit(1);
}

Value Visitor::visit(BinaryExpressionNode *node) {
    return Operations::performBinary(
        callAndGetValueFrom(node->expr1),
        node->op,
        callAndGetValueFrom(node->expr2)
    );
}

Value Visitor::visit(IntLiteralNode *node) {
    return Value(node->value);
}

Value Visitor::visit(FloatLiteralNode *node) {
    return Value(node->value);
}

Value Visitor::visit(BoolLiteralNode *node) {
    return Value(node->value);
}

Value Visitor::visit(StringLiteralNode *node) {
    return Value(std::string(node->value));
}

void Visitor::visit(IfNode *node) {
    Value condition = callAndGetValueFrom(node->condition);
    if (getBoolValue(condition)) {
        callVoidVisitOn(node->thenBlock);
    } else if (node->elseBlock) {
        callVoidVisitOn(node->elseBlock);
    }
}

void Visitor::visit(ForLoopNode *node) {
    pushNewSymbolFrame();
    callVoidVisitOn(node->init);
    Value condition = callAndGetValueFrom(node->condition);
    while (getBoolValue(condition)) {
        callVoidVisitOn(node->body);
        callVoidVisitOn(node->increment);
        condition = callAndGetValueFrom(node->condition);
    }
    popSymbolFrame();
}
