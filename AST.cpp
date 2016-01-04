#include "AST.hpp"
#include "Visitor.hpp"

enum class Type { INT, CHAR, BOOL, FLOAT };
enum class NodeType { STATEMENTS_LIST, ASSIGNMENT, PRINT, LOCATION, BINARY_EXPR, INT_LITERAL };

#define ACCEPT_VISITOR(CLASS) void CLASS::accept(Visitor *v) { v->visit(this); }

void ListNode::append(ASTNode *node) {
    list.push_back(node);
}

StatementsListNode::StatementsListNode() {
        nodeType = NodeType::STATEMENTS_LIST;
}

ACCEPT_VISITOR(StatementsListNode)

AssignmentNode::AssignmentNode(ASTNode *location, ASTNode *val):
locationNode(location),
value(val) {
    nodeType = NodeType::ASSIGNMENT;
}

ACCEPT_VISITOR(AssignmentNode)

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
