#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <cstdio>

class StatementsListNode;
class AssignmentNode;
class PrintNode;
class LocationNode;
class BinaryExpressionNode;
class IntLiteralNode;
class BoolLiteralNode;

class Value;

class Visitor {
public:
    Value* visit(StatementsListNode *);
    Value* visit(AssignmentNode *);
    Value* visit(PrintNode *);
    Value* visit(LocationNode *);
    Value* visit(BinaryExpressionNode *);
    Value* visit(IntLiteralNode *);
    Value* visit(BoolLiteralNode *);
};

#endif
