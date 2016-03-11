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
    bool isMutable;
    AssignmentNode(ASTNode *location, ASTNode *val, bool isAlsoDeclaration = false, bool isMutable = true);
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

class FloatLiteralNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    float value;
    FloatLiteralNode(float value);
};

class BoolLiteralNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    bool value;
    BoolLiteralNode(bool value);
};

class IfNode: public virtual ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    ASTNode *condition, *thenBlock, *elseBlock;
    IfNode(ASTNode *condition, ASTNode *thenBlock, ASTNode *elseBlock = nullptr);
};

#endif
