#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>

class Visitor;
class Value;

#define ACCEPT_VISITOR_DECLARE Value* accept(Visitor *v);

enum class Type;
enum class NodeType;

class ASTNode {
protected:
    Type type;
    NodeType nodeType;
public:
    virtual Value* accept(Visitor *v) = 0;
};

class ListNode {
public:
    std::vector<ASTNode *> list;

    void append(ASTNode *node);
};

class StatementsListNode: public virtual ASTNode, public ListNode {
public:
    ACCEPT_VISITOR_DECLARE

    StatementsListNode();
};

class AssignmentNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE
    ASTNode *locationNode;
    ASTNode *value;
    AssignmentNode(ASTNode *location, ASTNode *val);
};

class PrintNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    ASTNode *expr;
    PrintNode(ASTNode *expr);
};

class LocationNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    std::string id;
    LocationNode(std::string id);
};

class BinaryExpressionNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    ASTNode *expr1, *expr2;
    std::string op;

    BinaryExpressionNode(ASTNode *expr1, std::string op, ASTNode *expr2);
};

class IntLiteralNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    int value;
    IntLiteralNode(int value);
};

#endif
