#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <cstdio>
#include <list>
#include <map>

class StatementsListNode;
class BlockNode;
class AssignmentNode;
class DeclarationNode;
class PrintNode;
class LocationNode;
class BinaryExpressionNode;
class IntLiteralNode;
class FloatLiteralNode;
class BoolLiteralNode;
class IfNode;
class ForLoopNode;

class Value;

class Visitor {
protected:
    std::list< std::map<std::string, std::pair<Value*, bool> > > symbols;
public:
    Value* visit(StatementsListNode *);
    Value* visit(BlockNode *);
    Value* visit(AssignmentNode *);
    Value* visit(DeclarationNode *);
    Value* visit(PrintNode *);
    Value* visit(LocationNode *);
    Value* visit(BinaryExpressionNode *);
    Value* visit(IntLiteralNode *);
    Value* visit(FloatLiteralNode *);
    Value* visit(BoolLiteralNode *);
    Value* visit(IfNode *);
    Value* visit(ForLoopNode *);
    Visitor();
    void pushNewSymbolFrame();
    void popSymbolFrame();
};

#endif
