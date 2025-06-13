#ifndef VISITOR_H
#define VISITOR_H

class NumberNode;
class BinaryNode;

// visitor interface
class IVisitor {
public:
    IVisitor() = default;
    ~IVisitor() = default;
    virtual void visitNumberNode(const NumberNode *expr) = 0;
    virtual void visitBinaryNode(const BinaryNode *expr) = 0;
};

class NumberNodeVisitor : public IVisitor {
public:
    NumberNodeVisitor() = default;
    void visitNumberNode(const NumberNode *expr) override {
        return;
    }
};

class BinaryNodeVisitor : public IVisitor {
public:
    BinaryNodeVisitor() = default;
    void visitBinaryNode(const BinaryNode *expr) override {
        return;
    }
};

#endif // VISITOR_H
