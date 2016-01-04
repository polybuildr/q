#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <cstdio>

class StatementsListNode;
class AssignmentNode;
class PrintNode;
class LocationNode;
class BinaryExpressionNode;
class IntLiteralNode;

class Visitor {
public:
    void visit(StatementsListNode *);
    void visit(AssignmentNode *);
    void visit(PrintNode *);
    void visit(LocationNode *);
    void visit(BinaryExpressionNode *);
    void visit(IntLiteralNode *);
};

#endif
