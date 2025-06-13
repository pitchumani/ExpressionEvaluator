#include "ast.h"

void ASTPrinter::visitNumberNode(const NumberNode *expr) {
    os << expr->getValue();
}

void ASTPrinter::visitBinaryNode(const BinaryNode *expr) {
    os << "(" << expr->getOperator() << " ";
    expr->left->accept(this);
    os << " ";
    expr->right->accept(this);
    os << ")";
}

void ASTPrinter::print(const IExpression *expr) {
    expr->accept(this);
    os << std::endl;
}

