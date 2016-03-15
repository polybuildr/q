#include <map>
#include <unordered_map>
#include <list>
#include <memory>

#include "Visitor.hpp"
#include "AST.hpp"
#include "Value.hpp"
#include "Utils.cpp"

Value result;

std::shared_ptr<Value> smallInts[512];
std::shared_ptr<Value> trueValue;
std::shared_ptr<Value> falseValue;

void Visitor::pushNewSymbolFrame() {
    std::map<std::string, std::shared_ptr<Value> > frame;
    symbols.push_back(frame);
}

void Visitor::popSymbolFrame() {
    symbols.pop_back();
}

Visitor::Visitor() {
    trueValue = std::make_shared<Value>(true);
    falseValue = std::make_shared<Value>(false);
    for (int i = 0; i < 256; ++i) {
        smallInts[i] = std::make_shared<Value>(static_cast<int64_t>(i));
    }
    for (int i = 256; i < 512; ++i) {
        smallInts[i] = std::make_shared<Value>(static_cast<int64_t>(i - 512));
    }
    pushNewSymbolFrame();
}

std::shared_ptr<Value> Visitor::callVisitOn(ASTNode *node) {
    switch (node->nodeType) {
        case NodeType::LOCATION:
            return visit(static_cast<LocationNode *>(node));
            break;
        case NodeType::ASSIGNMENT:
            return visit(static_cast<AssignmentNode *>(node));
            break;
        case NodeType::BINARY_EXPR:
            return visit(static_cast<BinaryExpressionNode *>(node));
            break;
        case NodeType::DECLARATION:
            return visit(static_cast<DeclarationNode *>(node));
            break;
        case NodeType::STATEMENTS_LIST:
            return visit(static_cast<StatementsListNode *>(node));
            break;
        case NodeType::BLOCK:
            return visit(static_cast<BlockNode *>(node));
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
        case NodeType::IF:
            return visit(static_cast<IfNode *>(node));
            break;
        case NodeType::FOR:
            return visit(static_cast<ForLoopNode *>(node));
            break;
        case NodeType::PRINT:
            return visit(static_cast<PrintNode *>(node));
            break;
        default:
            printf("error: unknown node type, exiting\n");
            exit(1);
    }
    return nullptr;
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
    visit(static_cast<StatementsListNode *>(node->statementsList));
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(AssignmentNode *node) {
    std::string id = static_cast<LocationNode *>(node->locationNode)->id;
    if (!node->isAlsoDeclaration) {
        for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
            auto v = (*it).find(id);
            if (v != (*it).end()) {
                if (v->second->constant == true) {
                    printf("error: read-only variable '%s' is not assignable, exiting\n", id.c_str());
                    exit(1);
                }
                std::shared_ptr<Value> value(callVisitOn(node->value));
                v->second = value;
                return nullptr;
            }
        }
        printf("error: use of undeclared identifier '%s', exiting\n", id.c_str());
        exit(1);
    } else {
        std::shared_ptr<Value> value(callVisitOn(node->value));
        value->constant = !node->isMutable;
        symbols.back()[id] = value;
    }
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(DeclarationNode *node) {
    std::string id = static_cast<LocationNode *>(node->locationNode)->id;
    symbols.back()[id] = std::make_shared<Value>();
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(PrintNode *node) {
    std::shared_ptr<Value> value(callVisitOn(node->expr));
    value->print();
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(LocationNode *node) {
    for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
        if ((*it).find(node->id) != (*it).end()) {
            std::shared_ptr<Value> value((*it)[node->id]);
            if (value->type == ValueType::UNDEFINED) {
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
    Value value1(callVisitOn(node->expr1));
    Value value2(callVisitOn(node->expr2));

    Operations::performBinary(result, value1, node->op, value2);
    if (result.type == ValueType::INT) {
        if (result.data.num >= 0 && result.data.num < 256) {
            return smallInts[result.data.num];
        } else if (result.data.num < 0 && result.data.num >= -256) {
            return smallInts[512 + result.data.num];
        }
    } else if (result.type == ValueType::BOOL) {
        if (result.data.num) {
            return trueValue;
        } else {
            return falseValue;
        }
    }
    return std::make_shared<Value>(result);
}

std::shared_ptr<Value> Visitor::visit(IntLiteralNode *node) {
    if (node->value >= 0 && node->value < 256) {
        return smallInts[node->value];
    } else if (node->value < 0 && node->value >= -256) {
        return smallInts[512 + node->value];
    }
    return std::make_shared<Value>(node->value);
}

std::shared_ptr<Value> Visitor::visit(FloatLiteralNode *node) {
        return std::make_shared<Value>(node->value);
}

std::shared_ptr<Value> Visitor::visit(BoolLiteralNode *node) {
    if (node->value) {
        return trueValue;
    } else {
        return falseValue;
    }
}

std::shared_ptr<Value> Visitor::visit(IfNode *node) {
    std::shared_ptr<Value> expr(callVisitOn(node->condition));
    if (getBoolValue(expr)) {
        callVisitOn(node->thenBlock);
    } else if (node->elseBlock) {
        callVisitOn(node->elseBlock);
    }
    return nullptr;
}

std::shared_ptr<Value> Visitor::visit(ForLoopNode *node) {
    pushNewSymbolFrame();
    callVisitOn(node->init);
    std::shared_ptr<Value> condition(callVisitOn(node->condition));
    while (getBoolValue(condition)) {
        callVisitOn(node->body);
        callVisitOn(node->increment);
        condition = callVisitOn(node->condition);
    }
    popSymbolFrame();
    return nullptr;
}
