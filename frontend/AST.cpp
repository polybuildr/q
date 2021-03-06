#include <memory>
#include <cstring>

#include "AST.hpp"
#include "runtime/Visitor.hpp"

void ListNode::append(ASTNode *node) {
    list.push_back(node);
}

StatementsListNode::StatementsListNode() {
    nodeType = NodeType::STATEMENTS_LIST;
}

BlockNode::BlockNode(ASTNode *statementsList):
statementsList(statementsList) {
    nodeType = NodeType::BLOCK;
}

AssignmentNode::AssignmentNode(ASTNode *location, AssignOp op, ASTNode *val, bool isAlsoDeclaration, bool isMutable):
locationNode(location),
value(val),
isAlsoDeclaration(isAlsoDeclaration),
isMutable(isMutable),
op(op) {
    nodeType = NodeType::ASSIGNMENT;
}

DeclarationNode::DeclarationNode(ASTNode *location):
locationNode(location) {
    nodeType = NodeType::DECLARATION;
}

PrintNode::PrintNode(ASTNode *expr):
expr(expr) {
    nodeType = NodeType::PRINT;
}

LocationNode::LocationNode(std::string id):
id(id) {
    nodeType = NodeType::LOCATION;
}

BinaryExpressionNode::BinaryExpressionNode(ASTNode *expr1, BinaryOp op, ASTNode *expr2):
expr1(expr1),
expr2(expr2),
op(op) {
    nodeType = NodeType::BINARY_EXPR;
}

UnaryExpressionNode::UnaryExpressionNode(ASTNode *expr, char op):
expr(expr),
op(op) {
    nodeType = NodeType::UNARY_EXPR;
}

IntLiteralNode::IntLiteralNode(int64_t value):
value(value) {
    nodeType = NodeType::INT_LITERAL;
}

FloatLiteralNode::FloatLiteralNode(double value):
value(value) {
    nodeType = NodeType::FLOAT_LITERAL;
}

BoolLiteralNode::BoolLiteralNode(bool value):
value(value) {
    nodeType = NodeType::BOOL_LITERAL;
}

StringLiteralNode::StringLiteralNode(char *string) {
    nodeType = NodeType::STRING_LITERAL;
    value = string;
}

IfNode::IfNode(ASTNode* condition, ASTNode *thenBlock, ASTNode *elseBlock):
condition(condition),
thenBlock(thenBlock),
elseBlock(elseBlock) {
    nodeType = NodeType::IF;
};

ForLoopNode::ForLoopNode(ASTNode *init, ASTNode *condition, ASTNode *increment, ASTNode *body):
init(init),
condition(condition),
increment(increment),
body(body) {
    nodeType = NodeType::FOR;
}
