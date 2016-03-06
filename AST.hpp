#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>

class Visitor;
class Value;

#define ACCEPT_VISITOR_DECLARE Value* accept(Visitor *v);

enum class NodeType;

class ASTNode {
protected:
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

class BlockNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE
    ASTNode *statementsList;
    BlockNode(ASTNode*);
};

class AssignmentNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE
    ASTNode *locationNode;
    ASTNode *value;
    bool isAlsoDeclaration;
    AssignmentNode(ASTNode *location, ASTNode *val, bool isAlsoDeclaration = false);
};

class DeclarationNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE
    ASTNode *locationNode;
    DeclarationNode(ASTNode *location);
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

class BoolLiteralNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    bool value;
    BoolLiteralNode(bool value);
};

#endif
