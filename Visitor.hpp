#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <cstdio>
#include <vector>
#include <map>

#include "Globals.hpp"
#include "classes/String.hpp"

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
class StringLiteralNode;
class IfNode;
class ForLoopNode;

struct Value;

class Visitor {
protected:
    std::vector< std::map<std::string, Value > > symbols;
public:
    void visit(StatementsListNode *);
    void visit(BlockNode *);
    void visit(AssignmentNode *);
    void visit(DeclarationNode *);
    void visit(PrintNode *);
    void visit(LocationNode *, Value &result);
    void visit(BinaryExpressionNode *, Value &result);
    void visit(IntLiteralNode *, Value &result);
    void visit(FloatLiteralNode *, Value &result);
    void visit(BoolLiteralNode *, Value &result);
    void visit(StringLiteralNode *, Value &result);
    void visit(IfNode *);
    void visit(ForLoopNode *);
    Visitor();
    void pushNewSymbolFrame();
    void popSymbolFrame();
    void callVoidVisitOn(ASTNode *);
    void callAndGetValueFrom(ASTNode *, Value &result);
};

#endif
