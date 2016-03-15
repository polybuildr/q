#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <cstdio>
#include <vector>
#include <map>
#include <memory>

class ASTNode;
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
    std::vector< std::map<std::string, std::shared_ptr<Value> > > symbols;
public:
    std::shared_ptr<Value> visit(StatementsListNode *);
    std::shared_ptr<Value> visit(BlockNode *);
    std::shared_ptr<Value> visit(AssignmentNode *);
    std::shared_ptr<Value> visit(DeclarationNode *);
    std::shared_ptr<Value> visit(PrintNode *);
    std::shared_ptr<Value> visit(LocationNode *);
    std::shared_ptr<Value> visit(BinaryExpressionNode *);
    std::shared_ptr<Value> visit(IntLiteralNode *);
    std::shared_ptr<Value> visit(FloatLiteralNode *);
    std::shared_ptr<Value> visit(BoolLiteralNode *);
    std::shared_ptr<Value> visit(IfNode *);
    std::shared_ptr<Value> visit(ForLoopNode *);
    Visitor();
    void pushNewSymbolFrame();
    void popSymbolFrame();
    std::shared_ptr<Value> callVisitOn(ASTNode *);
};

#endif
