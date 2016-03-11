#include "AST.hpp"
#include "Visitor.hpp"

enum class NodeType {
    STATEMENTS_LIST,
    BLOCK,
    ASSIGNMENT,
    DECLARATION,
    PRINT,
    LOCATION,
    BINARY_EXPR,
    INT_LITERAL,
    FLOAT_LITERAL,
    BOOL_LITERAL,
    IF
};

#define ACCEPT_VISITOR(CLASS) Value* CLASS::accept(Visitor *v) { return v->visit(this); }

void ListNode::append(ASTNode *node) {
    list.push_back(node);
}

StatementsListNode::StatementsListNode() {
    nodeType = NodeType::STATEMENTS_LIST;
}

ACCEPT_VISITOR(StatementsListNode)

BlockNode::BlockNode(ASTNode *statementsList):
statementsList(statementsList) {
    nodeType = NodeType::BLOCK;
}

ACCEPT_VISITOR(BlockNode)

AssignmentNode::AssignmentNode(ASTNode *location, ASTNode *val, bool isAlsoDeclaration, bool isMutable):
locationNode(location),
value(val),
isMutable(isMutable),
isAlsoDeclaration(isAlsoDeclaration) {
    nodeType = NodeType::ASSIGNMENT;
}

ACCEPT_VISITOR(AssignmentNode)

DeclarationNode::DeclarationNode(ASTNode *location):
locationNode(location) {
    nodeType = NodeType::DECLARATION;
}

ACCEPT_VISITOR(DeclarationNode)

PrintNode::PrintNode(ASTNode *expr):
expr(expr) {
    nodeType = NodeType::PRINT;
}

ACCEPT_VISITOR(PrintNode)

LocationNode::LocationNode(std::string id):
id(id) {
    nodeType = NodeType::LOCATION;
}

ACCEPT_VISITOR(LocationNode)

BinaryExpressionNode::BinaryExpressionNode(ASTNode *expr1, std::string op, ASTNode *expr2):
expr1(expr1),
op(op),
expr2(expr2) {
    nodeType = NodeType::BINARY_EXPR;
}

ACCEPT_VISITOR(BinaryExpressionNode)

IntLiteralNode::IntLiteralNode(int value):
value(value) {
    nodeType = NodeType::INT_LITERAL;
}

ACCEPT_VISITOR(IntLiteralNode)

FloatLiteralNode::FloatLiteralNode(float value):
value(value) {
    nodeType = NodeType::FLOAT_LITERAL;
}

ACCEPT_VISITOR(FloatLiteralNode)

BoolLiteralNode::BoolLiteralNode(bool value):
value(value) {
    nodeType = NodeType::BOOL_LITERAL;
}

ACCEPT_VISITOR(BoolLiteralNode)

IfNode::IfNode(ASTNode* condition, ASTNode *block):
condition(condition),
block(block) {
    nodeType = NodeType::IF;
};

ACCEPT_VISITOR(IfNode)
