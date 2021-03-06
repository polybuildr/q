#ifndef AST_HPP
#define AST_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

class Visitor;
struct Value;

enum class NodeType {
    STATEMENTS_LIST,
    BLOCK,
    ASSIGNMENT,
    DECLARATION,
    PRINT,
    LOCATION,
    BINARY_EXPR,
    UNARY_EXPR,
    INT_LITERAL,
    FLOAT_LITERAL,
    BOOL_LITERAL,
    STRING_LITERAL,
    IF,
    FOR,
};

enum class BinaryOp {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    GREATER_THAN,
    LESS_THAN,
    GREATER_THAN_OR_EQUAL,
    LESS_THAN_OR_EQUAL,
    LOGICAL_AND,
    LOGICAL_OR,
};

enum class AssignOp {
    SIMPLE,
    COMPOUND_SUM,
    COMPOUND_DIFFERENCE,
    COMPOUND_PRODUCT,
    COMPOUND_QUOTIENT,
};

class ASTNode {
public:
    NodeType nodeType;
};

class ListNode {
public:
    std::vector<ASTNode *> list;

    void append(ASTNode *node);
};

class StatementsListNode: public ASTNode, public ListNode {
public:
    StatementsListNode();
};

class BlockNode: public ASTNode {
public:
    ASTNode *statementsList;
    BlockNode(ASTNode*);
};

class AssignmentNode: public ASTNode {
public:
    ASTNode *locationNode;
    ASTNode *value;
    bool isAlsoDeclaration;
    bool isMutable;
    AssignOp op;
    AssignmentNode(ASTNode *location, AssignOp op, ASTNode *val, bool isAlsoDeclaration = false, bool isMutable = true);
};

class DeclarationNode: public ASTNode {
public:
    ASTNode *locationNode;
    DeclarationNode(ASTNode *location);
};

class PrintNode: public ASTNode {
public:
    ASTNode *expr;
    PrintNode(ASTNode *expr);
};

class LocationNode: public ASTNode {
public:
    std::string id;
    LocationNode(std::string id);
};

class BinaryExpressionNode: public ASTNode {
public:
    ASTNode *expr1, *expr2;
    BinaryOp op;

    BinaryExpressionNode(ASTNode *expr1, BinaryOp op, ASTNode *expr2);
};

class UnaryExpressionNode: public ASTNode {
public:
    ASTNode *expr;
    char op;

    UnaryExpressionNode(ASTNode *expr, char op);
};

class IntLiteralNode: public ASTNode {
public:
    int64_t value;
    IntLiteralNode(int64_t value);
};

class FloatLiteralNode: public ASTNode {
public:
    double value;
    FloatLiteralNode(double value);
};

class BoolLiteralNode: public ASTNode {
public:
    bool value;
    BoolLiteralNode(bool value);
};

class StringLiteralNode: public ASTNode {
public:
    char *value;
    StringLiteralNode(char *string);
};

class IfNode: public ASTNode {
public:
    ASTNode *condition, *thenBlock, *elseBlock;
    IfNode(ASTNode *condition, ASTNode *thenBlock, ASTNode *elseBlock = nullptr);
};

class ForLoopNode: public ASTNode {
public:
    ASTNode *init, *condition, *increment, *body;
    ForLoopNode(ASTNode *init, ASTNode *condition, ASTNode *increment, ASTNode *body);
};

#endif
