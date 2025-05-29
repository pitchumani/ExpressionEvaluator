#include "expression.h"
#include <gtest/gtest.h>
#include <stdexcept>

// Helper functions to create nodes
NumberNode* makeNumber(int val) {
    return new NumberNode(val);
}

BinaryNode* makeBinary(Operator op, NumberNode* left, NumberNode* right) {
    return new BinaryNode(op, left, right);
}

TEST(NumberNodeTest, EvaluateReturnsTrue) {
    NumberNode n(42);
    EXPECT_TRUE(n.evaluate());
}

TEST(NumberNodeTest, GetValueReturnsValue) {
    NumberNode n(123);
    EXPECT_EQ(n.getValue(), 123);
}

TEST(BinaryNodeTest, EvaluateReturnsTrueForValidChildren) {
    NumberNode* l = makeNumber(1);
    NumberNode* r = makeNumber(2);
    BinaryNode b(Operator::ADD, l, r);
    EXPECT_TRUE(b.evaluate());
    delete l;
    delete r;
}

TEST(BinaryNodeTest, AddOperation) {
    NumberNode* l = makeNumber(10);
    NumberNode* r = makeNumber(5);
    BinaryNode b(Operator::ADD, l, r);
    EXPECT_EQ(b.getValue(), 15);
    delete l;
    delete r;
}

TEST(BinaryNodeTest, SubOperation) {
    NumberNode* l = makeNumber(10);
    NumberNode* r = makeNumber(5);
    BinaryNode b(Operator::SUB, l, r);
    EXPECT_EQ(b.getValue(), 5);
    delete l;
    delete r;
}

TEST(BinaryNodeTest, MulOperation) {
    NumberNode* l = makeNumber(3);
    NumberNode* r = makeNumber(4);
    BinaryNode b(Operator::MUL, l, r);
    EXPECT_EQ(b.getValue(), 12);
    delete l;
    delete r;
}

TEST(BinaryNodeTest, DivOperation) {
    NumberNode* l = makeNumber(20);
    NumberNode* r = makeNumber(4);
    BinaryNode b(Operator::DIV, l, r);
    EXPECT_EQ(b.getValue(), 5);
    delete l;
    delete r;
}

TEST(BinaryNodeTest, DivByZeroThrows) {
    NumberNode* l = makeNumber(10);
    NumberNode* r = makeNumber(0);
    BinaryNode b(Operator::DIV, l, r);
    EXPECT_THROW(b.getValue(), std::runtime_error);
    delete l;
    delete r;
}

TEST(BinaryNodeTest, UnknownOperatorThrows) {
    NumberNode* l = makeNumber(1);
    NumberNode* r = makeNumber(2);
    BinaryNode b(static_cast<Operator>(999), l, r);
    EXPECT_THROW(b.getValue(), std::runtime_error);
    delete l;
    delete r;
}

// Additional tests

TEST(BinaryNodeTest, NestedBinaryNodes) {
    NumberNode* l = makeNumber(2);
    NumberNode* r = makeNumber(3);
    BinaryNode* add = new BinaryNode(Operator::ADD, l, r); // 2 + 3 = 5
    NumberNode* n = makeNumber(4);
    BinaryNode mul(Operator::MUL, add, n); // (2 + 3) * 4 = 20
    EXPECT_EQ(mul.getValue(), 20);
    delete add;
    delete n;
}

TEST(BinaryNodeTest, EvaluateReturnsFalseIfLeftChildFails) {
    struct FailingNode : public NumberNode {
        FailingNode() : NumberNode(0) {}
        bool evaluate() override { return false; }
    };
    NumberNode* r = makeNumber(1);
    FailingNode* l = new FailingNode();
    BinaryNode b(Operator::ADD, l, r);
    EXPECT_FALSE(b.evaluate());
    delete l;
    delete r;
}

TEST(BinaryNodeTest, EvaluateReturnsFalseIfRightChildFails) {
    struct FailingNode : public NumberNode {
        FailingNode() : NumberNode(0) {}
        bool evaluate() override { return false; }
    };
    NumberNode* l = makeNumber(1);
    FailingNode* r = new FailingNode();
    BinaryNode b(Operator::ADD, l, r);
    EXPECT_FALSE(b.evaluate());
    delete l;
    delete r;
}

TEST(NumberNodeTest, NegativeValue) {
    NumberNode n(-42);
    EXPECT_EQ(n.getValue(), -42);
}

TEST(BinaryNodeTest, ZeroAddZero) {
    NumberNode* l = makeNumber(0);
    NumberNode* r = makeNumber(0);
    BinaryNode b(Operator::ADD, l, r);
    EXPECT_EQ(b.getValue(), 0);
    delete l;
    delete r;
}

TEST(BinaryNodeTest, LargeNumbersMultiplication) {
    NumberNode* l = makeNumber(100000);
    NumberNode* r = makeNumber(200000);
    BinaryNode b(Operator::MUL, l, r);
    EXPECT_EQ(b.getValue(), 20000000000LL);
    delete l;
    delete r;
}