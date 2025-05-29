#ifndef TEST_EXPRESSION_H_
#define TEST_EXPRESSION_H_

#include "expression.h"
#include <cassert>
#include <iostream>
#include <sstream>

// Helper function to test NumberNode
void test_NumberNode() {
    NumberNode n1(42);
    assert(n1.getValue() == 42);
    assert(n1.getTypeName() == "NumberNode");
    std::ostringstream oss;
    n1.print(oss);
    assert(oss.str() == "42");
}

// Helper function to test BinaryNode with addition
void test_BinaryNode_add() {
    IExpression* left = ExpressionFactory::createNumber(10);
    IExpression* right = ExpressionFactory::createNumber(32);
    IExpression* expr = ExpressionFactory::createBinary(Operator::ADD, left, right);
    assert(expr->getTypeName() == "BinaryNode");
    assert(expr->evaluate()); // Should call evaluate on children
    assert(expr->getValue() == 42);
    std::ostringstream oss;
    expr->print(oss);
    assert(oss.str() == "(10 + 32)");
    delete expr;
    delete left;
    delete right;
}

// Helper function to test BinaryNode with subtraction
void test_BinaryNode_sub() {
    IExpression* left = ExpressionFactory::createNumber(50);
    IExpression* right = ExpressionFactory::createNumber(8);
    IExpression* expr = ExpressionFactory::createBinary(Operator::SUB, left, right);
    assert(expr->getValue() == 42);
    std::ostringstream oss;
    expr->print(oss);
    assert(oss.str() == "(50 - 8)");
    delete expr;
    delete left;
    delete right;
}

void test_RootNode() {
    IExpression* num = ExpressionFactory::createNumber(7);
    // If RootNode is abstract, create a simple concrete subclass for testing
    struct TestRootNode : public RootNode {
        using RootNode::RootNode;
        // Implement any pure virtual methods if needed
        virtual std::string getTypeName() const override { return "RootNode"; }
        virtual int evaluate() override { return getValue(); }
        virtual int getValue() const override { return child->getValue(); }
        virtual void print(std::ostream& os) const override { os << "RootNode:"; child->print(os); }
    };
    TestRootNode root(num);
    assert(root.getValue() == 7);
    std::ostringstream oss;
    root.print(oss);
    assert(oss.str().find("RootNode:") != std::string::npos);
    delete num;
}
}

// Run all tests
void run_expression_tests() {
    test_NumberNode();
    test_BinaryNode_add();
    test_BinaryNode_sub();
    test_RootNode();
    std::cout << "All expression tests passed!" << std::endl;
}

#endif // TEST_EXPRESSION_H_