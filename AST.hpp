#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <vector>
#include <string>

class Visitor;
class Value;

#define ACCEPT_VISITOR_DECLARE std::shared_ptr<Value> accept(Visitor *v);

enum class NodeType;

class ASTNode {
protected:
    NodeType nodeType;
public:
    virtual std::shared_ptr<Value> accept(Visitor *v) {
        return nullptr;
    }
};

class ListNode {
public:
    std::vector<ASTNode *> list;

    void append(ASTNode *node);
};

class StatementsListNode: public ASTNode, public ListNode {
public:
    ACCEPT_VISITOR_DECLARE

    StatementsListNode();
};

class BlockNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE
    ASTNode *statementsList;
    BlockNode(ASTNode*);
};

class AssignmentNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE
    ASTNode *locationNode;
    ASTNode *value;
    bool isAlsoDeclaration;
    bool isMutable;
    AssignmentNode(ASTNode *location, ASTNode *val, bool isAlsoDeclaration = false, bool isMutable = true);
};

class DeclarationNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE
    ASTNode *locationNode;
    DeclarationNode(ASTNode *location);
};

class PrintNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    ASTNode *expr;
    PrintNode(ASTNode *expr);
};

class LocationNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    std::string id;
    LocationNode(std::string id);
};

class BinaryExpressionNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    ASTNode *expr1, *expr2;
    std::string op;

    BinaryExpressionNode(ASTNode *expr1, std::string op, ASTNode *expr2);
};

class IntLiteralNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    int value;
    IntLiteralNode(int value);
};

class FloatLiteralNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    float value;
    FloatLiteralNode(float value);
};

class BoolLiteralNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    bool value;
    BoolLiteralNode(bool value);
};

class IfNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    ASTNode *condition, *thenBlock, *elseBlock;
    IfNode(ASTNode *condition, ASTNode *thenBlock, ASTNode *elseBlock = nullptr);
};

class ForLoopNode: public ASTNode {
public:
    ACCEPT_VISITOR_DECLARE

    ASTNode *init, *condition, *increment, *body;
    ForLoopNode(ASTNode *init, ASTNode *condition, ASTNode *increment, ASTNode *body);
};

#endif
