#ifndef EXPRESSION_H_
#define EXPRESSION_H_

// Expression interface
class IExpression {
public:
    virtual void evaluate() = 0;
};

enum class Operator {
    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    ABS,
    SIN,
    COS
};

class NumberNode : public IExpression {
    int val = 0;
public:
    explicit NumberNode(int val) : val(val) {}
    void evaluate() override;
};

class BinaryNode : public IExpression {
    Operator op;
    IExpression *left;
    IExpression *right;
public:
    BinaryNode(Operator op, IExpression *left, IExpression *right) :
        op(op), left(left), right(right) {}
};

class AddNode : public BinaryNode {
public:
    explicit AddNode(IExpression *left, IExpression *right) :
        BinaryNode(Operator::ADD, left, right) {}
    void evaluate() override;
    
};

#endif  // EXPRESSION_H_
