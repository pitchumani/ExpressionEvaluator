#include "expression.h"

bool NumberNode::evaluate() {
    // do nothing
    return true;
}

int64_t NumberNode::getValue() {
    return val;
}

bool BinaryNode::evaluate() {
    // TODO: evaluate the input expressions and give error
    return left->evaluate() && right->evaluate();
}

int64_t BinaryNode::getValue() {
    switch (op) {
    case Operator::ADD: return left->getValue() + right->getValue();
    case Operator::SUB: return left->getValue() - right->getValue();
    case Operator::MUL: return left->getValue() * right->getValue();
    case Operator::DIV:
    {
        auto rhsValue = right->getValue();
        if (rhsValue == 0) {
            throw std::runtime_error("Divide by zero");
        }
        return left->getValue() / rhsValue;
    }
    default:
        throw std::runtime_error("Unknown operator");
    }
}

