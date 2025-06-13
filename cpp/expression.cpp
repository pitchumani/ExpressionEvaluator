#include "expression.h"

bool RootNode::evaluate() {
    return root->evaluate();
}

int64_t RootNode::getValue() const {
    return root->getValue();
}

void RootNode::print(std::ostream &os) const {
    os << "RootNode: ";
    root->print(os);
}

bool NumberNode::evaluate() {
    // do nothing
    return true;
}

int64_t NumberNode::getValue() const {
    return val;
}

void NumberNode::print(std::ostream &os) const {
    os << val;
}

void NumberNode::accept(IVisitor *v) const {
    v->visitNumberNode(this);
}

bool BinaryNode::evaluate() {
    // TODO: evaluate the input expressions and give error
    return left->evaluate() && right->evaluate();
}

int64_t BinaryNode::getValue() const {
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

void BinaryNode::print(std::ostream &os) const {
    os << "(";
    left->print(os);
    os << opToChar(op);
    right->print(os);
    os << ")";
}

void BinaryNode::accept(IVisitor *v) const {
    v->visitBinaryNode(this);
}

