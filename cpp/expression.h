#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include<string>

// Expression interface
class IExpression {
public:
    virtual ~IExpression() = default;
    virtual bool evaluate() = 0;
    virtual int getValue() = 0;
    virtual std::string getTypeName() = 0;
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
};

class NumberNode : public IExpression {
    int val = 0;
public:
    explicit NumberNode(int val) : val(val) {}
    bool evaluate() override;
    int getValue() override;
    std::string getTypeName() override { return std::string("NumberNode"); }
};

class BinaryNode : public IExpression {
protected:
    Operator op;
    std::unique_ptr<IExpression> left;
    std::unique_ptr<IExpression> right;
public:
    BinaryNode(Operator op, std::unique_ptr<IExpression> left,
            std::unique_ptr<IExpression> right) :
        op(op), left(std::move(left)), right(std::move(right)) {}
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
};

class ExpressionFactory {
public:
    static std::unique_ptr<IExpression> createNumber(int value) {
        return std::make_unique<NumberNode>(value);
    }
    static std::unique_ptr<BinaryNode> createBinary(Operator op,
            std::unique_ptr<IExpression> left,
            std::unique_ptr<IExpression> right) {
        return std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
    }
};

#endif  // EXPRESSION_H_
