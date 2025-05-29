#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include<string>
#include<iostream>

// Expression interface
class IExpression {
public:
    virtual ~IExpression() = default;
    virtual bool evaluate() = 0;
    virtual int getValue() = 0;
    virtual std::string getTypeName() = 0;
    virtual void print(std::ostream &os) const = 0;
    template<typename T> T to() { return dynamic_cast<T>(this); }
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

class RootNode : public IExpression {
    IExpression *root;

public:
    RootNode(IExpression *expr) : root(expr) {}
    bool evaluate() override {
        return root->evaluate();
    }
    int getValue() override {
        return root->getValue();
    }
    void print(std::ostream &os) const override {
        os << "RootNode: ";
        root->print(os);
    }
};

class NumberNode : public IExpression {
    int val = 0;
public:
    explicit NumberNode(int val) : val(val) {}
    bool evaluate() override;
    int getValue() override;
    std::string getTypeName() override { return std::string("NumberNode"); }
    void print(std::ostream &os) const override {
        os << val;
    }
};

class BinaryNode : public IExpression {
protected:
    Operator op;
    IExpression *left;
    IExpression *right;
public:
    BinaryNode(Operator op, IExpression *left, IExpression *right) :
        op(op), left(left), right(right) {}
    bool evaluate() override;
    int getValue() override;
    std::string getTypeName() override { return std::string("BinaryNode"); }
    static Operator get(char op) {
        if (op == '+') return Operator::ADD;
        else if (op == '-') return Operator::SUB;
        else if (op == '*') return Operator::MUL;
        else if (op == '/') return Operator::DIV;
        throw std::runtime_error("Unknown operator");
    }
    void print(std::ostream &os) const override {
        os << "(";
        left->print(os);
        os << " " << opToChar(op) << " ";
        right->print(os);
        os << ")";
    }
private:
    static char opToChar(Operator op) {
        switch (op) {
            case Operator::ADD: return '+';
            case Operator::SUB: return '-';
            case Operator::MUL: return '*';
            case Operator::DIV: return '/';
            default: return '?';  // Unknown operator
        }
    }
};

class ExpressionFactory {
public:
    static IExpression* createNumber(int value) {
        return new NumberNode(value);
    }
    static IExpression* createBinary(Operator op,
            IExpression* left,
            IExpression* right) {
        return new BinaryNode(op, left, right);
    }
};

#endif  // EXPRESSION_H_
