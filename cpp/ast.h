#ifndef AST_H_
#define AST_H_

#include <iostream>

#include "visitor.h"
#include "expression.h"

class ASTPrinter : public IVisitor {
    std::ostream &os;
public:
    ASTPrinter(std::ostream &os) : os(os) {}
    void visitNumberNode(const NumberNode *expr);
    void visitBinaryNode(const BinaryNode *expr);
    void print(const IExpression *expr);
};

#endif // AST_H_

